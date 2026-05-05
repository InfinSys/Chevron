
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file atomic_free_list.hpp
 *
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
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
 * TODO: INCOMPLETE DOCUMENTATION!!!
 * 
 * @details
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
class AtomicFreeList {
	// ===================================================================================== //
    //      <> chevron::memory::AtomicFreeList | [PRIVATE] NESTED TYPES
    // ===================================================================================== //

	/*!
	 * @brief
	 * TODO: INCOMPLETE DOCUMENTATION!!!
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
	 * TODO: INCOMPLETE DOCUMENTATION!!!
	 * 
	 * @details
	 * TODO: INCOMPLETE DOCUMENTATION!!!
	 */
	[[nodiscard]] FreeRegionNode* peek();

	/*!
	 * @brief
	 * TODO: INCOMPLETE DOCUMENTATION!!!
	 *
	 * @details
	 * TODO: INCOMPLETE DOCUMENTATION!!!
	 */
	void push(FreeRegionNode* begin_node, FreeRegionNode* end_node);

	/*!
	 * @brief
	 * TODO: INCOMPLETE DOCUMENTATION!!!
	 *
	 * @details
	 * TODO: INCOMPLETE DOCUMENTATION!!!
	 */
	FreeRegionNode* pop(size_t quantity);

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
	 * TODO: INCOMPLETE DOCUMENTATION!!!
	 *
	 * @details
	 * TODO: INCOMPLETE DOCUMENTATION!!!
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
