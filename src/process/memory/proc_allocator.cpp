
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

// ===================================================================================== //
//      <> chevron::process::ProcessMemoryAllocator | CONSTRUCTORS / DESTRUCTOR
// ===================================================================================== //

chevron::process::ProcessMemoryAllocator::ProcessMemoryAllocator(const size_t maxAllocs)
    : descriptors_{nullptr}, chunkCount_{0}, maxChunks_{maxAllocs}
{
    if (maxAllocs == 0) {
        throw std::invalid_argument{
            "ProcessMemoryAllocator: OS bound allocation limit cannot be zero."
        };
    }

    descriptors_ = new memory::ChunkDescriptor[maxChunks_];
}

chevron::process::ProcessMemoryAllocator::~ProcessMemoryAllocator() noexcept
{
    // Release each allocation back to OS
    for (size_t i = 0; i < chunkCount_; i++) {
        // descriptors_[i].base();
    }

    // Delete allocation descriptor array
    if (descriptors_) delete[] descriptors_;
}
