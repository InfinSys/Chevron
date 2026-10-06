
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file free_list.cpp
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 09-26-2026
 */

#include "chevron/memory/free_list.hpp"
#include "chevron/memory/memory_defs.hpp"

using chevron::memory::FreeList;
using chevron::memory::FreeRegionNode;
using chevron::memory::FreeRegionChain;

// ===================================================================================== //
//      <> chevron::memory::FreeList | CONSTRUCTORS / DESTRUCTOR
// ===================================================================================== //

FreeList::FreeList() noexcept
    : free_list_head_{ListHead{nullptr}}
{
    //
}

FreeList::FreeList(FreeList&& other) noexcept
    : free_list_head_{other.free_list_head_}
{
    other.free_list_head_ = ListHead{nullptr};
}

// ===================================================================================== //
//      <> chevron::memory::FreeList | [PUBLIC] MEMBER METHODS
// ===================================================================================== //

const FreeRegionNode* FreeList::peek()
{
    return free_list_head_;
}

void FreeList::push(FreeRegionChain& batch)
{
    batch.tail()->next = free_list_head_;
    free_list_head_ = batch.head();
    batch.clear();
}

FreeRegionChain FreeList::pop(size_t quantity)
{
    if (free_list_head_ == nullptr) {
        return FreeRegionChain{};
    }

    FreeRegionNode* batchHead = free_list_head_;
    FreeRegionNode* batchTail = batchHead;
    size_t grabCount = 1;

    while (grabCount < quantity) {
        batchTail = static_cast<FreeRegionNode*>(batchTail->next);
        if (batchTail == nullptr) return FreeRegionChain{};
        grabCount++;
    }

    free_list_head_ = static_cast<FreeRegionNode*>(batchTail->next);
    batchTail->next = nullptr;
    return FreeRegionChain{batchHead, batchTail, grabCount};
}
