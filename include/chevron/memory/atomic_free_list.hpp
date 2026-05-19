
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file atomic_free_list.hpp
 *
 * @brief
 * Lock-free embedded free list with platform-specific
 * 16-byte CAS.
 * 
 * @details
 * Provides a shared lock-free free list implementation
 * used across Chevron's memory hierarchy. Operations
 * are backed by platform-specific atomic intrinsics
 * to guarantee inline double-width compare-and-swap.
 * 
 * @note
 * To achieve 16-byte atomics on the x86-64 architecture
 * with GCC, we are using `__sync_val_compare_and_swap`
 * rather than `__atomic_compare_exchange` because GCC
 * dispatches __atomic_* for 16-byte types through
 * libatomic rather than inlining the lock cmpxchg16b
 * instruction. The __sync_* family inlines the instruction
 * directly with the `-mcx16` flag, providing a verifiable
 * lock-free guarantee. The decision was made to take this
 * route to allow independent verification of the pool's
 * lock-free capabilities via assembly output.
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 05-05-2026
 */

#ifndef CHEVRON_LIB_ATOMIC_MEMORY_FREE_LIST_H_
#define CHEVRON_LIB_ATOMIC_MEMORY_FREE_LIST_H_

#include "chevron/memory/memory_defs.hpp"
#include "chevron/thread/utility/tagged_ptr.hpp"

namespace chevron::memory
{

/*!
 * @brief
 * Lock-free embedded memory block free list.
 * 
 * @details
 * Manages a singly-linked list of free region nodes
 * through an ABA-safe tagged pointer head. Push and
 * pop operations use platform-specific atomic
 * intrinsics to guarantee lock-free behavior without
 * relying on std::atomic for 16-byte types.
 */
class AtomicFreeList {
	// ===================================================================================== //
    //      <> chevron::memory::AtomicFreeList | [PRIVATE] NESTED TYPES
    // ===================================================================================== //

	/*!
	 * @brief
	 * ABA-safe cyclic count pointer.
	 * 
	 * @warning
	 * This is not an explicit atomic type, it is
	 * implicitly treated as such within the confines
	 * of this class.
	 */
	using ListHead = utility::TaggedPointer<void>;

	// ===================================================================================== //
    //      <> chevron::memory::AtomicFreeList | CONSTRUCTORS / DESTRUCTOR
    // ===================================================================================== //
public:
	/*! @brief Construct empty free list. */
	AtomicFreeList() noexcept;

	AtomicFreeList(const AtomicFreeList&) = delete;

	/*! @brief Move free list. */
	AtomicFreeList(AtomicFreeList&& other) noexcept;

	~AtomicFreeList() noexcept = default;

	// ===================================================================================== //
    //      <> chevron::memory::AtomicFreeList | [PUBLIC] MEMBER METHODS
    // ===================================================================================== //

	/*!
	 * @brief
	 * Returns current head node without removing it
	 * from list.
	 * 
	 * @details
	 * Returns a non-mutable reference to the free lists
	 * head. While this has no effect on the state of the
	 * free list, it certainly does require synchronization
	 * to retrieve the up-to-date head of the list.
	 * 
	 * @return
	 * Pointer to front node or nullptr if empty
	 */
	[[nodiscard]] const FreeRegionNode* peek();

	/*!
	 * @brief
	 * Pushes a chain of free region nodes onto list
	 * head.
	 *
	 * @details
	 * Links provided chain's tail to current head and
	 * atomically installs chain's first node as new
	 * head via platform-specific CAS.
	 */
	void push(FreeRegionNode* begin_node, FreeRegionNode* end_node);

	/*!
	 * @brief
	 * Pops a chain of free region nodes from list
	 * head.
	 *
	 * @details
	 * Walks list to find Nth node, then atomically
	 * detaches entire chain via platform-specific
	 * CAS. Returns nullptr if insufficient nodes
	 * are available.
	 * 
	 * @return
	 * Head of detached chain or nullptr if insufficient nodes
	 */
	[[nodiscard]] FreeRegionNode* pop(size_t quantity);

	// ===================================================================================== //
    //      <> chevron::memory::AtomicFreeList | OPERATORS
    // ===================================================================================== //

	AtomicFreeList& operator=(const AtomicFreeList&) = delete;

	AtomicFreeList& operator=(AtomicFreeList&&) = delete;

	// ===================================================================================== //
    //      <> chevron::memory::AtomicFreeList | [PRIVATE] ATTRIBUTES
    // ===================================================================================== //
private:
	ListHead free_list_head_;   ///< Head of the embedded free list (ABA-safe)

	// ===================================================================================== //
    //      <> chevron::memory::AtomicFreeList | [PRIVATE] MEMBER METHODS
    // ===================================================================================== //

	/*!
	 * @brief
	 * Atomically loads current list head value.
	 *
	 * @details
	 * Uses platform-specific intrinsics to perform
	 * an atomic 16-byte read of the tagged pointer.
	 * 
	 * @return
	 * Free list head
	 */
	[[nodiscard]] ListHead atomic_load();

	// ===================================================================================== //
    //      <> chevron::memory::AtomicFreeList | COMPILE-TIME GUARANTEES
    // ===================================================================================== //

    static_assert(
        sizeof(ListHead) == alignof(ListHead) &&
        alignof(ListHead) == 16,
		"AtomicFreeList requires lock-free 16-byte atomics for a free "
        "list pointer. The free list type must not exceed 16 bytes in "
        "size and must be aligned to a 16-byte address."
    );
};

}

#endif // CHEVRON_LIB_ATOMIC_MEMORY_FREE_LIST_H_
