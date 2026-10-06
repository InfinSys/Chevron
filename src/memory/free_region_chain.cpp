
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file free_region_chain.cpp
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 09-29-2026
 */

#include "chevron/memory/free_region_chain.hpp"
#include "chevron/memory/memory_defs.hpp"
#include <stdexcept>

using chevron::memory::FreeRegionNode;
using chevron::memory::FreeRegionChain;

// ===================================================================================== //
//      <> chevron::memory::FreeRegionChain | CONSTRUCTORS / DESTRUCTOR
// ===================================================================================== //

FreeRegionChain::FreeRegionChain() noexcept
    : head_{nullptr},
    tail_{nullptr},
    length_{0}
{
    //
}

FreeRegionChain::FreeRegionChain(FreeRegionNode* solo_region) noexcept
    : head_{solo_region},
    tail_{solo_region},
    length_{1}
{
    if (solo_region == nullptr) {
        length_ = 0;
    }
}

FreeRegionChain::FreeRegionChain(
    FreeRegionNode* chain_head,
    FreeRegionNode* chain_tail,
    size_t length
)
    : head_{chain_head},
    tail_{chain_tail},
    length_{length}
{
    if (length == 0 && (chain_head || chain_tail)) {
        throw std::invalid_argument{
            "FreeRegionChain: Chain length specified as zero but endpoint "
            "nodes are present."
        };
    }

    if (length > 0 && (!chain_head || !chain_tail)) {
        throw std::invalid_argument{
            "FreeRegionChain: Non-zero chain length requires non-null "
            "endpoint nodes."
        };
    }

    if (length == 1 && chain_head != chain_tail) {
        throw std::invalid_argument{
            "FreeRegionChain: Chain length of one requires identical "
            "head and tail endpoints."
        };
    }

    if (length > 1 && chain_head == chain_tail) {
        throw std::invalid_argument{
            "FreeRegionChain: Improper termination of node chain."
        };
    }
}

FreeRegionChain::FreeRegionChain(FreeRegionChain&& other) noexcept
    : head_{other.head_},
    tail_{other.tail_},
    length_{other.length_}
{
    other.clear();
}

// ===================================================================================== //
//      <> chevron::memory::FreeRegionChain | [PUBLIC] MEMBER METHODS
// ===================================================================================== //

bool FreeRegionChain::isEmpty() const noexcept
{
    return head_ == nullptr && tail_ == nullptr;
}

size_t FreeRegionChain::length() const noexcept
{
    return length_;
}

bool FreeRegionChain::isValidChain() const noexcept
{
    if (length_ == 0) {
        return head_ == nullptr && tail_ == nullptr;
    }

    if (head_ == nullptr || tail_ == nullptr) {
        return false;
    }

    const FreeRegionNode* node = head_;
    size_t remaining = length_;

    // Follow nodes to end of chain (very expensive)
    while (remaining > 1) {
        if (node == nullptr || node == tail_) {
            return false;
        }

        node = static_cast<const FreeRegionNode*>(node->next);
        --remaining;
    }

    return node == tail_ && node->next == nullptr;
}

FreeRegionNode* FreeRegionChain::head() noexcept
{
    return head_;
}

FreeRegionNode* FreeRegionChain::tail() noexcept
{
    return tail_;
}

void FreeRegionChain::clear() noexcept
{
    head_ = nullptr;
    tail_ = nullptr;
    length_ = 0;
}
