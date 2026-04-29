
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file proc_allocator.hpp
 *
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 04-28-2026
 */

#ifndef CHEVRON_LIB_PROCESS_MEMORY_ALLOCATOR_H_
#define CHEVRON_LIB_PROCESS_MEMORY_ALLOCATOR_H_

#include <atomic>
#include "chevron/memory/chunk_descriptor.hpp"

namespace chevron::process
{

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 *
 * @details
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
class ProcessMemoryAllocator {
    // ===================================================================================== //
    //      <> chevron::process::ProcessMemoryAllocator | CONSTRUCTORS / DESTRUCTOR
    // ===================================================================================== //
public:
    /*!
     * @brief
     * Construct process memory allocator with maximum allocation count.
     */
    explicit ProcessMemoryAllocator(const size_t maxAllocs);

    ProcessMemoryAllocator(const ProcessMemoryAllocator&) = delete;

    ProcessMemoryAllocator(ProcessMemoryAllocator&&) = delete;

    ~ProcessMemoryAllocator() noexcept;

    // ===================================================================================== //
    //      <> chevron::process::ProcessMemoryAllocator | OPERATORS
    // ===================================================================================== //

    ProcessMemoryAllocator& operator=(const ProcessMemoryAllocator&) = delete;

    ProcessMemoryAllocator& operator=(ProcessMemoryAllocator&&) = delete;

    // ===================================================================================== //
    //      <> chevron::process::ProcessMemoryAllocator | [PUBLIC] MEMBER METHODS
    // ===================================================================================== //

    // Start...

    // ===================================================================================== //
    //      <> chevron::process::ProcessMemoryAllocator | [PRIVATE] ATTRIBUTES
    // ===================================================================================== //
private:
    memory::ChunkDescriptor* descriptors_;   ///< Fixed array of chunk descriptors
    std::atomic<size_t> chunkCount_;         ///< Count of chunks currently acquired
    size_t maxChunks_;                       ///< Capacity of chunk descriptor array
};

}

#endif // CHEVRON_LIB_PROCESS_MEMORY_ALLOCATOR_H_
