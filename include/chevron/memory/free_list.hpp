
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file free_list.hpp
 *
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 *
 * @details
 * TODO: INCOMPLETE DOCUMENTATION!!!
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 09-26-2026
 */

#ifndef CHEVRON_LIB_MEMORY_FREE_LIST_H_
#define CHEVRON_LIB_MEMORY_FREE_LIST_H_

#include "chevron/memory/memory_defs.hpp"
#include "chevron/memory/free_region_chain.hpp"

namespace chevron::memory
{

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 *
 * @details
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
class FreeList {
    // ===================================================================================== //
    //      <> chevron::memory::FreeList | [PRIVATE] NESTED TYPES
    // ===================================================================================== //

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     *
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    using ListHead = FreeRegionNode*;

    // ===================================================================================== //
    //      <> chevron::memory::FreeList | CONSTRUCTORS / DESTRUCTOR
    // ===================================================================================== //
public:
    /*! @brief Construct empty free list. */
    FreeList() noexcept;

    FreeList(const FreeList&) = delete;

    /*! @brief Move free list. */
    FreeList(FreeList&& other) noexcept;

    ~FreeList() noexcept = default;

    // ===================================================================================== //
    //      <> chevron::memory::FreeList | [PUBLIC] MEMBER METHODS
    // ===================================================================================== //

    /*!
     * @brief
     * Returns current head node without removing it
     * from list.
     *
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    [[nodiscard]] const FreeRegionNode* peek();

    /*!
     * @brief
     * Pushes a chain of free region nodes onto list
     * head.
     *
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    void push(FreeRegionChain& batch);

    /*!
     * @brief
     * Pops a chain of free region nodes from list
     * head.
     *
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    [[nodiscard]] FreeRegionChain pop(size_t quantity);

    // ===================================================================================== //
    //      <> chevron::memory::FreeList | OPERATORS
    // ===================================================================================== //

    FreeList& operator=(const FreeList&) = delete;

    FreeList& operator=(FreeList&&) = delete;

    // ===================================================================================== //
    //      <> chevron::memory::FreeList | [PRIVATE] ATTRIBUTES
    // ===================================================================================== //
private:
    ListHead free_list_head_;   ///< Head of embedded free list
};

}

#endif // CHEVRON_LIB_MEMORY_FREE_LIST_H_
