
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file proc_mem_pool.cpp
 *
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 04-14-2026
 */

#include <thread>
#include "chevron/process/memory/proc_mem_pool.hpp"
#include "chevron/utility/bits/alignment.hpp"
#include "chevron/utility/scope/scope_guard.hpp"

#if CHEVRON_MSVC
  #include <intrin.h>
#endif

using chevron::process::ProcessMemoryPool;
using chevron::memory::FreeRegionNode;
using chevron::memory::MemoryRegion;
using chevron::utility::ScopeGuard;
using chevron::units::Bytes;

// ===================================================================================== //
//      <> chevron::process::ProcessMemoryPool | CONSTRUCTORS / DESTRUCTOR
// ===================================================================================== //

ProcessMemoryPool::ProcessMemoryPool(const MemoryPoolConfig& config)
	: free_list_head_{TaggedPointer{nullptr, 0}},
	bytes_acquired_{0},
	blocks_per_chunk_{0},
	config_{config},
	allocator_{1}  // Placeholder
{
	config_.isValidOrThrow();

	compute_effective_block_geometry();
	compute_effective_chunk_geometry();
	validate_budget_constraints();
	init_thread_cache_configuration();
	reinit_memory_allocator();
	is_lock_free_or_throw();
}

ProcessMemoryPool::~ProcessMemoryPool() noexcept
{
	//
}

// ===================================================================================== //
//      <> chevron::process::ProcessMemoryPool | [PUBLIC] MEMBER METHODS
// ===================================================================================== //

Bytes ProcessMemoryPool::distribution_size() const noexcept
{
	return config_.block_size;
}

Bytes ProcessMemoryPool::acquisition_size() const noexcept
{
	return config_.chunk_size;
}

size_t ProcessMemoryPool::alignment_guarantee() const noexcept
{
	return config_.block_alignment;
}

size_t ProcessMemoryPool::chunk_count() const noexcept
{
	return bytes_acquired_.load(std::memory_order_relaxed) / config_.chunk_size.size_t_bytes();
}

size_t ProcessMemoryPool::max_chunk_count() const noexcept
{
	return config_.budget_ceiling / config_.chunk_size;
}

Bytes ProcessMemoryPool::bytes_in_possession() const noexcept
{
	return Bytes{bytes_acquired_.load(std::memory_order_relaxed)};
}

MemoryRegion ProcessMemoryPool::allocate()
{
	// ----->  (Fast Path)  <-----------------------------------------------------------
	// 
	// Attempt to source memory from thread local memory cache, avoiding the
	// need for thread synchronization on allocation requests.

	ThreadLocalMemoryCache& localMemory = get_current_thread_cache();

	if (localMemory.has_memory_free()) {
		FreeRegionNode* block = localMemory.free_list_head;
		localMemory.free_list_head = static_cast<FreeRegionNode*>(block->next);
		localMemory.free_blocks--;
		return MemoryRegion{block, config_.block_alignment, config_.block_size};
	}

	// ----->  (Slow Path)  <-----------------------------------------------------------
	// 
	// Attempt to source memory from the process-wide shared free list to refill
	// this thread local memory cache. This path of execution requires thread
	// synchronization to successfully acquire memory.

	const size_t blockBatchSize = localMemory.compute_growth_batch(); // NOTE: This can't be zero
	FreeRegionNode* batchAllocChain = pop_batch(blockBatchSize);

	// ----->  (Extremely Slow Path)  <-------------------------------------------------
	// 
	// Attempt to source memory from the operating system since both the thread
	// local memory cache and process-wide shared free list are exhausted. This
	// path of execution will require further thread synchronization and possibly
	// a momentary surrender of control to the kernel. In this circumstance, you
	// are refilling both the process shared free list and this thread local
	// memory.

	if (batchAllocChain == nullptr) {
		expand_memory();
		batchAllocChain = pop_batch(blockBatchSize);

		if (batchAllocChain == nullptr)
			throw std::bad_alloc{};
	}

	// ----->  (Thread Memory Refill)  <------------------------------------------------

	FreeRegionNode* block = batchAllocChain;
	localMemory.free_list_head = static_cast<FreeRegionNode*>(block->next);
	localMemory.cached_blocks += blockBatchSize;
	localMemory.free_blocks += blockBatchSize - 1;
	block->next = nullptr;
	return MemoryRegion{block, config_.block_alignment, config_.block_size};
}

void ProcessMemoryPool::deallocate(const MemoryRegion& block) noexcept
{
	ThreadLocalMemoryCache& localMemory = get_current_thread_cache();
	FreeRegionNode* returnedBlock = static_cast<FreeRegionNode*>(block.base());

	returnedBlock->next = localMemory.free_list_head;
	localMemory.free_list_head = returnedBlock;
	localMemory.free_blocks++;

	if (localMemory.cached_blocks > config_.max_thread_blocks) {
		const size_t drainCount = localMemory.compute_shrink_batch();

		FreeRegionNode* returnChainHead = localMemory.free_list_head;
		FreeRegionNode* returnChainTail = returnChainHead;

		for (size_t i = 1; i < drainCount; i++)
			returnChainTail = static_cast<FreeRegionNode*>(returnChainTail->next);

		localMemory.free_list_head = static_cast<FreeRegionNode*>(returnChainTail->next);

		returnChainTail->next = nullptr;
		localMemory.free_blocks -= drainCount;
		localMemory.cached_blocks -= drainCount;
		push_batch(returnChainHead, returnChainTail);
	}
}

// ===================================================================================== //
//      <> chevron::process::ProcessMemoryPool | [PRIVATE] MEMBER METHODS
// ===================================================================================== //

void ProcessMemoryPool::compute_effective_block_geometry()
{
	const Bytes minBlockSize{sizeof(FreeRegionNode)};

	if (config_.block_size < minBlockSize)
		config_.block_size = minBlockSize;

	const size_t alignment = config_.block_alignment;
	const size_t rawByteSize = config_.block_size.size_t_bytes();

	config_.block_size = Bytes{bits::alignUp(rawByteSize, alignment)};
}

void ProcessMemoryPool::compute_effective_chunk_geometry()
{
	const size_t blockSizeBytes = config_.block_size.size_t_bytes();
	const size_t chunkSizeBytes = config_.chunk_size.size_t_bytes();

	config_.chunk_size = Bytes{bits::alignUp(chunkSizeBytes, blockSizeBytes)};
	blocks_per_chunk_ = config_.chunk_size / config_.block_size;
}

void ProcessMemoryPool::validate_budget_constraints()
{
#if CHEVRON_64BIT
	return;  // NOTE: Do nothing for now...
#elif CHEVRON_32BIT
	if (config_.budget_ceiling >= units::GiB{4}) {
		throw std::invalid_argument{
			"ProcessMemoryPool: Cannot address more than 4 GiB "
			"of memory on 32-bit platforms."
		};
	}
#endif
}

void ProcessMemoryPool::reinit_memory_allocator()
{
	const size_t maxAllocs = config_.budget_ceiling / config_.chunk_size;
	allocator_.~ProcessMemoryAllocator();
	new (&allocator_) ProcessMemoryAllocator{maxAllocs};
}

void ProcessMemoryPool::init_thread_cache_configuration()
{
	// ON HOLD (no use found)
}

void ProcessMemoryPool::is_lock_free_or_throw()
{
	if (!bytes_acquired_.is_lock_free() || !expansion_state_.is_lock_free())
		throw std::bad_exception{}; // NOTE: This exception type is not it cheif...

#if CHEVRON_CLANG || CHEVRON_GCC
	if (!free_list_head_.is_lock_free())
		throw std::bad_exception{}; // NOTE: This exception type is not it cheif...
#endif
}

ProcessMemoryPool::ThreadLocalMemoryCache& ProcessMemoryPool::get_current_thread_cache() noexcept
{
	thread_local ThreadLocalMemoryCache threadCache{
		.free_list_head = nullptr,
		.cached_blocks = 0,
		.batch_size = config_.initial_thread_blocks,
		.shared_pool = this
	};

	return threadCache;
}

void ProcessMemoryPool::carve_and_link(const MemoryRegion& chunk)
{
	const size_t blockByteSize = config_.block_size.size_t_bytes();

	std::byte* cursor = static_cast<std::byte*>(chunk.base());
	std::byte* end = cursor + chunk.size().size_t_bytes();

	FreeRegionNode* chainHead = static_cast<FreeRegionNode*>(chunk.base());
	FreeRegionNode* chainTail = nullptr;

	while (cursor < end) {
		FreeRegionNode* currentBlock = reinterpret_cast<FreeRegionNode*>(cursor);
		std::byte* nextBlockAddr = cursor + blockByteSize;

		if (nextBlockAddr < end) {
			currentBlock->next = reinterpret_cast<FreeRegionNode*>(nextBlockAddr);
		}
		else {
			currentBlock->next = nullptr;
			chainTail = currentBlock;
		}

		cursor = nextBlockAddr;
	}

	push_batch(chainHead, chainTail);
}

void ProcessMemoryPool::push_batch(FreeRegionNode* head, FreeRegionNode* tail)
{
	//
	// BE AWARE: Concurrent Zone Below
	//

	bool swapSuccess = false;
	TaggedPointer newHead{head};

#if CHEVRON_MSVC
	TaggedPointer currentHead{nullptr, 0};
	_InterlockedCompareExchange128(
		reinterpret_cast<volatile int64_t*>(&free_list_head_),
		0,
		0,
		reinterpret_cast<int64_t*>(&currentHead)
	);
#else
	TaggedPointer currentHead = free_list_head_.load(std::memory_order_acquire);
#endif

	do {
		tail->next = currentHead.ptr;
		newHead.swaps = currentHead.swaps + 1;

#if CHEVRON_MSVC
		swapSuccess = _InterlockedCompareExchange128(
			reinterpret_cast<volatile int64_t*>(&free_list_head_),
			static_cast<int64_t>(newHead.swaps),
			reinterpret_cast<int64_t>(newHead.ptr),
			reinterpret_cast<int64_t*>(&currentHead)
		) == 1;
#else
		swapSuccess = free_list_head_.compare_exchange_weak(
			currentHead,                 ///< Expected current head
			newHead,                     ///< New head to install if expected still present
			std::memory_order_release,   ///< Success: publish node write to other threads
			std::memory_order_acquire    ///< Failure: acquire updated head for next attempt
		);
#endif
	} while (!swapSuccess/* CAS-Loop */);
}

FreeRegionNode* ProcessMemoryPool::pop_batch(size_t batch_size)
{
	//
	// BE AWARE: Concurrent Zone Below
	//

	bool swapSuccess = false;
	size_t grabCount = 0;
	FreeRegionNode* batchHead = nullptr;
	FreeRegionNode* batchTail = nullptr;

#if CHEVRON_MSVC
	TaggedPointer currentHead{nullptr, 0};
	_InterlockedCompareExchange128(
		reinterpret_cast<volatile int64_t*>(&free_list_head_),
		0,
		0,
		reinterpret_cast<int64_t*>(&currentHead)
	);
#else
	TaggedPointer currentHead = free_list_head_.load(std::memory_order_acquire);
#endif

	do {
		if (currentHead.ptr == nullptr) return nullptr;
		batchHead = static_cast<FreeRegionNode*>(currentHead.ptr);
		batchTail = batchHead;

		grabCount = 1;

		while (grabCount < batch_size) {
			batchTail = static_cast<FreeRegionNode*>(batchTail->next);
			if (batchTail == nullptr) return nullptr;
			grabCount++;
		}

		TaggedPointer newHead{batchTail->next, currentHead.swaps + 1};

#if CHEVRON_MSVC
		swapSuccess = _InterlockedCompareExchange128(
			reinterpret_cast<volatile int64_t*>(&free_list_head_),
			static_cast<int64_t>(newHead.swaps),
			reinterpret_cast<int64_t>(newHead.ptr),
			reinterpret_cast<int64_t*>(&currentHead)
		) == 1;
#else
		swapSuccess = free_list_head_.compare_exchange_weak(
			currentHead,                 ///< Expected current head
			newHead,                     ///< New head to install if expected still present
			std::memory_order_release,   ///< Success: publish node write to other threads
			std::memory_order_acquire    ///< Failure: acquire updated head for next attempt
		);
#endif
	} while (!swapSuccess/* CAS-Loop */);

	batchTail->next = nullptr;
	return batchHead;
}

// paste

void ProcessMemoryPool::expand_memory()
{
	//
	// BE AWARE: Concurrent Zone Below
	//

	///---------------------------------------------------------------------------------
    // ----->  PHASE 1 | (Concurrent Expansion Gate)  <---------------------------------
    // 
	// Atomically claim exclusive ownership of the expansion path. Only
	// one thread may perform OS memory acquisition at a time. The first
	// thread to arrive transitions the gate from idle to expanding and
	// proceeds. All other threads yield until that expansion completes,
	// then return to a replinished process-wide free list.

	ExpansionState expectedState = ExpansionState::IDLE;
	const ExpansionState attemptState = ExpansionState::EXPANDING;
	const bool expansionOwner = expansion_state_.compare_exchange_strong(
		expectedState,
		attemptState,
		std::memory_order_acquire,
		std::memory_order_relaxed
	);

	if (!expansionOwner) {
		while (expansion_state_.load(std::memory_order_acquire) == attemptState)
			std::this_thread::yield();
		return; // Memory is now available
	}

	// Restore idle expansion state on scope exit beyond this point
	[[maybe_unused]] ScopeGuard expansionStateRestore{
		[&]() {
			expansion_state_.store(ExpansionState::IDLE, std::memory_order_release);
		}
	};

	///---------------------------------------------------------------------------------
    // ----->  PHASE 2 | (Occupancy Budget Gate)  <-------------------------------------
    // 
	// Atomically increment the number of bytes acquired by the chunk size.
	// If the result exceeds the budget ceiling, roll back and throw. This
	// check happens before the allocator call so we don't waste an OS
	// allocation on a chunk we can't afford anyway.

	const size_t chunkByteSize = config_.chunk_size.size_t_bytes();
	const size_t maxMemoryBytes = config_.budget_ceiling.size_t_bytes();
	const size_t previousBytesAcq = bytes_acquired_.fetch_add(
		chunkByteSize,
		std::memory_order_acq_rel
	);

	// Decrement byte acquisition count on failure beyond this point
	ScopeGuard byteIncrementGuard{
		[&]() {
			bytes_acquired_.fetch_sub(chunkByteSize, std::memory_order_relaxed);
		}
	};

	if (previousBytesAcq + chunkByteSize > maxMemoryBytes)
		throw std::bad_alloc{};

	///---------------------------------------------------------------------------------
	// ----->  PHASE 3 | (Chunk Acquisition)  <-----------------------------------------
	// 
	// Delegate to the allocator for the OS allocation. If this fails,
	// roll back the byte budget and rethrow. The allocator independently
	// enforces its own acquisition count ceiling.

	MemoryRegion newChunk = allocator_.acquire_chunk(
		config_.chunk_size,
		config_.block_alignment
	);

	///---------------------------------------------------------------------------------
	// ----->  PHASE 4 | (Chunk Integration)  <-----------------------------------------
	// 
	// Carve the acquired chunk into blocks, thread them into a chain, and
	// prepend the chain onto the shared free list.

	byteIncrementGuard.dismiss();
	carve_and_link(newChunk);
}
