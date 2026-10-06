
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
  * @file free_region_chain.hpp
  *
  * @brief
  * TODO: INCOMPLETE DOCUMENTATION!!!
  *
  * @author
  * Jamon T. Bailey
  *
  * @date 09-29-2026
  */

#ifndef CHEVRON_LIB_MEMORY_FREE_REGION_CHAIN_H_
#define CHEVRON_LIB_MEMORY_FREE_REGION_CHAIN_H_

#include <cstddef>
#include "chevron/memory/memory_defs.hpp"

namespace chevron::memory
{

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
    explicit FreeRegionChain(FreeRegionNode* solo_region) noexcept;
    /*! @brief Construct free-region node chain. */
    FreeRegionChain(FreeRegionNode* chain_head, FreeRegionNode* chain_tail, size_t length);
    /*! @brief Transfer free-region node chain to new instance. */
    FreeRegionChain(FreeRegionChain&& other) noexcept;

    FreeRegionChain(const FreeRegionChain&) = delete;

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
    [[nodiscard]] bool isValidChain() const noexcept;

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
    void clear() noexcept;

    // ===================================================================================== //
    //      <> chevron::memory::FreeRegionChain | OPERATORS
    // ===================================================================================== //

    FreeRegionChain& operator=(const FreeRegionChain&) = delete;
    FreeRegionChain& operator=(FreeRegionChain&&) = delete;
};

}

#endif // CHEVRON_LIB_MEMORY_FREE_REGION_CHAIN_H_
