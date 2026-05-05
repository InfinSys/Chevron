
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file atomic_free_list.cpp
 *
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 05-05-2026
 */

#include "chevron/memory/atomic_free_list.hpp"

#if CHEVRON_MSVC
  #include <intrin.h>
#elif CHEVRON_GCC
  #include <string.h>
#endif

using chevron::memory::AtomicFreeList;
using chevron::memory::FreeRegionNode;

// ===================================================================================== //
//      <> chevron::memory::AtomicFreeList | CONSTRUCTORS / DESTRUCTOR
// ===================================================================================== //

AtomicFreeList::AtomicFreeList() noexcept
	: free_list_head_{ListHead{nullptr, 0}}
{
	//
}

AtomicFreeList::AtomicFreeList(AtomicFreeList&& other) noexcept
	: free_list_head_{other.free_list_head_}
{
	other.free_list_head_ = ListHead{nullptr, 0};
}

// ===================================================================================== //
//      <> chevron::memory::AtomicFreeList | [PUBLIC] MEMBER METHODS
// ===================================================================================== //

FreeRegionNode* AtomicFreeList::peek()
{
	//
	// BE AWARE: Concurrent Zone Below
	//

	ListHead currentHead = atomic_load();
	return static_cast<FreeRegionNode*>(currentHead.ptr);
}

void AtomicFreeList::push(FreeRegionNode* begin_node, FreeRegionNode* end_node)
{
	//
	// BE AWARE: Concurrent Zone Below
	//

	bool swapSuccess = false;
	ListHead newHead{begin_node};
	ListHead currentHead = atomic_load();

	do {
		end_node->next = currentHead.ptr;
		newHead.swaps = currentHead.swaps + 1;

#if CHEVRON_MSVC
		swapSuccess = _InterlockedCompareExchange128(
			reinterpret_cast<volatile int64_t*>(&free_list_head_),
			static_cast<int64_t>(newHead.swaps),
			reinterpret_cast<int64_t>(newHead.ptr),
			reinterpret_cast<int64_t*>(&currentHead)
		) == 1;
#elif CHEVRON_GCC
		{
			__int128_t currentHeadRaw = __sync_val_compare_and_swap(
				reinterpret_cast<__int128*>(&free_list_head_),   ///< CAS target base address
				*reinterpret_cast<__int128*>(&currentHead),      ///< Expected current head
				*reinterpret_cast<__int128*>(&newHead)           ///< New head if expected still present
			);

			swapSuccess = (
				currentHeadRaw == *reinterpret_cast<__int128*>(&currentHead)
			);

			if (!swapSuccess) // Use updated head for next attempt
				memcpy(&currentHead, &currentHeadRaw, sizeof(currentHead));
		}
#elif CHEVRON_CLANG
		swapSuccess = __atomic_compare_exchange(
			&free_list_head_,   ///< CAS target base address
			&currentHead,       ///< Expected current head
			&newHead,           ///< New head if expected still present
			true,               ///< Use weak compare exchange
			__ATOMIC_RELEASE,   ///< Success: publish node write to other threads
			__ATOMIC_ACQUIRE    ///< Failure: acquire updated head for next attempt
		);
#endif
	} while (!swapSuccess/* CAS-Loop */);
}

FreeRegionNode* AtomicFreeList::pop(size_t quantity)
{
	//
	// BE AWARE: Concurrent Zone Below
	//

	bool swapSuccess = false;
	size_t grabCount = 0;
	FreeRegionNode* batchHead = nullptr;
	FreeRegionNode* batchTail = nullptr;

	ListHead currentHead = atomic_load();

	do {
		if (currentHead.ptr == nullptr) return nullptr;
		batchHead = static_cast<FreeRegionNode*>(currentHead.ptr);
		batchTail = batchHead;

		grabCount = 1;

		while (grabCount < quantity) {
			batchTail = static_cast<FreeRegionNode*>(batchTail->next);
			if (batchTail == nullptr) return nullptr;
			grabCount++;
		}

		ListHead newHead{batchTail->next, currentHead.swaps + 1};

#if CHEVRON_MSVC
		swapSuccess = _InterlockedCompareExchange128(
			reinterpret_cast<volatile int64_t*>(&free_list_head_),
			static_cast<int64_t>(newHead.swaps),
			reinterpret_cast<int64_t>(newHead.ptr),
			reinterpret_cast<int64_t*>(&currentHead)
		) == 1;
#elif CHEVRON_GCC
		{
			__int128_t currentHeadRaw = __sync_val_compare_and_swap(
				reinterpret_cast<__int128*>(&free_list_head_),   ///< CAS target base address
				*reinterpret_cast<__int128*>(&currentHead),      ///< Expected current head
				*reinterpret_cast<__int128*>(&newHead)           ///< New head if expected still present
			);

			swapSuccess = (
				currentHeadRaw == *reinterpret_cast<__int128*>(&currentHead)
			);

			if (!swapSuccess) // Acquire updated head for next attempt
				memcpy(&currentHead, &currentHeadRaw, sizeof(currentHead));
		}
#elif CHEVRON_CLANG
		swapSuccess = __atomic_compare_exchange(
			&free_list_head_,   ///< CAS target base address
			&currentHead,       ///< Expected current head
			&newHead,           ///< New head if expected still present
			true,               ///< Use weak compare exchange
			__ATOMIC_RELEASE,   ///< Success: publish node write to other threads
			__ATOMIC_ACQUIRE    ///< Failure: acquire updated head for next attempt
		);
#endif
	} while (!swapSuccess/* CAS-Loop */);

	batchTail->next = nullptr;
	return batchHead;
}

// ===================================================================================== //
//      <> chevron::memory::AtomicFreeList | [PRIVATE] MEMBER METHODS
// ===================================================================================== //

AtomicFreeList::ListHead AtomicFreeList::atomic_load()
{
	//
	// BE AWARE: Concurrent Zone Below
	//

	ListHead currentHead{nullptr, 0};

#if CHEVRON_MSVC
	_InterlockedCompareExchange128(
		reinterpret_cast<volatile int64_t*>(&free_list_head_),
		0,
		0,
		reinterpret_cast<int64_t*>(&currentHead)
	);
#elif CHEVRON_GCC
	{
		__int128_t currentHeadRaw = __sync_val_compare_and_swap(
			reinterpret_cast<__int128*>(&free_list_head_),
			0,
			0
		);

		memcpy(&currentHead, &currentHeadRaw, sizeof(currentHead));
	}
#elif CHEVRON_CLANG
	__atomic_load(&free_list_head_, &currentHead, __ATOMIC_ACQUIRE);
#endif

	return currentHead;
}
