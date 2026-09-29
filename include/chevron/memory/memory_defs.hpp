
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
  * @file memory_defs.hpp
  *
  * @brief
  * Core type definitions for Chevron's memory
  * infrastructure.
  *
  * @author
  * Jamon T. Bailey
  *
  * @date 04-30-2026
  */

#ifndef CHEVRON_LIB_MEMORY_DEFINITIONS_H_
#define CHEVRON_LIB_MEMORY_DEFINITIONS_H_

#include <cstdint>
#include <atomic>

namespace chevron::memory
{

/*!
 * @brief
 * Embedded free list node for overlaying on free memory
 * blocks.
 * 
 * @details
 * When a block of memory is free, its first bytes are
 * reinterpreted as this structure, threading it into a
 * free list.
 */
struct FreeRegionNode {
	void* next;   ///< Pointer to next free block
};

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 * 
 * @details
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
class FreeRegionChain {
    // ===================================================================================== //
    //      <> chevron::memory::FreeRegionChain | [PRIVATE] ATTRIBUTES
    // ===================================================================================== //

    FreeRegionNode* head_; ///< Head of free region chain
    FreeRegionNode* tail_; ///< Tail of free region chain
    size_t length_;        ///< Number of nodes in chain

    // ===================================================================================== //
    //      <> chevron::memory::FreeRegionChain | CONSTRUCTORS / DESTRUCTOR
    // ===================================================================================== //
public:
    /*! @brief Construct empty free-region node chain. */
    FreeRegionChain() noexcept;
    /*! @brief Construct single free-region node chain. */
    FreeRegionChain(FreeRegionNode* solo_region) noexcept;
    /*! @brief Construct free-region node chain. */
    FreeRegionChain(FreeRegionNode* chain_head, FreeRegionNode* chain_tail, size_t length) noexcept;

    ~FreeRegionChain() = default;

    // ===================================================================================== //
    //      <> chevron::memory::FreeRegionChain | [PUBLIC] MEMBER METHODS
    // ===================================================================================== //

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     *
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    [[nodiscard]] bool isValid() const noexcept;

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     *
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    [[nodiscard]] bool isEmpty() const noexcept;

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     *
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    [[nodiscard]] size_t length() const noexcept;

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     *
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    [[nodiscard]] FreeRegionNode* head() noexcept;

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     *
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    [[nodiscard]] FreeRegionNode* tail() noexcept;

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     *
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    void invalidate() noexcept;
};

}

#endif // CHEVRON_LIB_MEMORY_DEFINITIONS_H_
