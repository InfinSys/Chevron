
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file proc_allocator.cpp
 *
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 04-28-2026
 */

#include <exception>
#include "chevron/process/memory/proc_allocator.hpp"
#include "chevron/common/macro_defs.h"

#if CHEVRON_WINDOWS
  #include <malloc.h>   ///< _aligned_malloc / _aligned_free
#elif CHEVRON_MACOS || CHEVRON_LINUX
  #include <cstdlib>    ///< std::aligned_alloc / std::free
#endif

// ===================================================================================== //
//      <> chevron::process::ProcessMemoryAllocator | CONSTRUCTORS / DESTRUCTOR
// ===================================================================================== //

chevron::process::ProcessMemoryAllocator::ProcessMemoryAllocator(const size_t maxAllocs)
    : allocListHead_{nullptr}, chunkCount_{0}, maxAllocs_{maxAllocs}
{
    if (maxAllocs == 0) {
        throw std::invalid_argument{
            "ProcessMemoryAllocator: OS bound allocation limit cannot be zero."
        };
    }
}

chevron::process::ProcessMemoryAllocator::~ProcessMemoryAllocator() noexcept
{
    AllocationNode* currentAlloc = allocListHead_.load(std::memory_order_relaxed);

    while (currentAlloc != nullptr) {
        AllocationNode* nextAlloc = currentAlloc->next;

        // Deallocate chunk of memory
        deallocate_aligned_memory(currentAlloc->descriptor);
        // Delete allocation tracking node
        delete currentAlloc;

        currentAlloc = nextAlloc;
    }
}

// ===================================================================================== //
//      <> chevron::process::ProcessMemoryAllocator | [PUBLIC] MEMBER METHODS
// ===================================================================================== //

size_t chevron::process::ProcessMemoryAllocator::acquisition_count() const noexcept
{
    return chunkCount_.load(std::memory_order_relaxed);
}

size_t chevron::process::ProcessMemoryAllocator::max_acquisition_count() const noexcept
{
    return maxAllocs_;
}

chevron::memory::MemoryRegion chevron::process::ProcessMemoryAllocator::acquire_chunk(
    units::Bytes size, size_t alignment
)
{
    // BE AWARE: Concurrent Zone Below

    ///---------------------------------------------------------------------------------
    // ----->  PHASE 1 | (Budget Gate)  <-----------------------------------------------
    // 
    // Atomically increment the chunk count to reserve permission for a
    // new acquisition. If the incremented value exceeds the ceiling,
    // roll back and throw. A successful increment only means you may
    // attempt an OS allocation and nothing more.

    size_t previousAllocCount = chunkCount_.fetch_add(1, std::memory_order_acq_rel);

    if (previousAllocCount >= maxAllocs_) {
        chunkCount_.fetch_sub(1, std::memory_order_relaxed);
        throw std::bad_alloc{};
    }

    ///---------------------------------------------------------------------------------
    // ----->  PHASE 2 | (Memory Acquisition)  <----------------------------------------
    // 
    // Request aligned memory from the OS. If this fails, roll back the
    // chunk counter and rethrow. The tracking list is untouched and no
    // node was created, nor was a CAS attempted.

    void* newAllocBase = nullptr;

    try {
        newAllocBase = allocate_aligned_memory(size, alignment);
    }
    catch (...) {
        chunkCount_.fetch_sub(1, std::memory_order_relaxed);
        throw;
    }

    ///---------------------------------------------------------------------------------
    // ----->  PHASE 3 | (Acquisition Tracking)  <--------------------------------------
    // 
    // Allocate a tracking node via regular new and populate it with the
    // chunk's descriptor data. The node is fully initialized before it
    // is pushed onto the list because there should be no thread that
    // observes a partially constructed node.

    AllocationNode* newAllocNode_ptr = new AllocationNode{
        memory::ChunkDescriptor{newAllocBase, alignment, size},
        nullptr
    };

    ///---------------------------------------------------------------------------------
    // ----->  PHASE 4 | (Publish Acquisition)  <---------------------------------------
    // 
    // Prepend the node to the list head via compare-and-swap. Read the
    // current head into the node's next pointer. Attempt to swap the
    // head from the current value to the new node. If another thread
    // pushed between the read and the swap, compare_exchange_weak fails
    // and updates node->next to the new head automatically for another
    // try.

    newAllocNode_ptr->next = allocListHead_.load(std::memory_order_acquire);

    while (
        !allocListHead_.compare_exchange_weak(
            newAllocNode_ptr->next,      ///< Expected current head
            newAllocNode_ptr,            ///< New head to install if expected still present
            std::memory_order_release,   ///< Success: publish node write to other threads
            std::memory_order_relaxed    ///< Failure: retry with corrected expectation
        )
    ) { /* CAS-loop */ }

    return memory::MemoryRegion{newAllocBase, alignment, size};
}


// ===================================================================================== //
//      <> chevron::process::ProcessMemoryAllocator | [PRIVATE] MEMBER METHODS
// ===================================================================================== //

void* chevron::process::ProcessMemoryAllocator::allocate_aligned_memory(units::Bytes size, size_t alignment)
{
    void* newAlloc = nullptr;

#if CHEVRON_WINDOWS
    newAlloc = _aligned_malloc(size.size_t_bytes(), alignment);
#elif CHEVRON_MACOS || CHEVRON_LINUX
    // NOTE: The size better be a multiple of the alignment
    newAlloc = std::aligned_alloc(alignment, size.size_t_bytes());
#endif

    if (!newAlloc) throw std::bad_alloc{};
    return newAlloc;
}

void chevron::process::ProcessMemoryAllocator::deallocate_aligned_memory(const memory::ChunkDescriptor& chunk) noexcept
{
#if CHEVRON_WINDOWS
    _aligned_free(chunk.base());
#elif CHEVRON_MACOS || CHEVRON_LINUX
    std::free(chunk.base());
#endif
}
