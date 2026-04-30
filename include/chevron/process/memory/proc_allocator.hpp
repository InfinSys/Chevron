
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file proc_allocator.hpp
 *
 * @brief
 * OS memory acquisition and chunk tracking for process.
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
#include "chevron/memory/region.hpp"

namespace chevron::process
{

/*!
 * @brief
 * Process-wide lock-free OS memory aquisition.
 *
 * @details
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
class ProcessMemoryAllocator {
    // ===================================================================================== //
    //      <> chevron::Callable | [PRIVATE] NESTED TYPES
    // ===================================================================================== //
    
    /*!
     * @brief
     * Tracking node for individual OS bound allocations.
     * 
     * @details
     * This type is internal to the allocator. It adds the linked
     * list machinery that does not belong on the general-purpose
     * descriptor type. Every successful OS-bound allocation that
     * occurs results in the construction of one of these nodes.
     */
    struct AllocationNode {
        /*! @brief Describes the acquired chunk of memory. */
        memory::ChunkDescriptor descriptor;
        /*! @brief Next node in allocation tracking list. */
        AllocationNode* next;
    };

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     * 
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    using AllocListHead = std::atomic<AllocationNode*>;
    
    // ===================================================================================== //
    //      <> chevron::process::ProcessMemoryAllocator | CONSTRUCTORS / DESTRUCTOR
    // ===================================================================================== //
public:
    /*!
     * @brief
     * Construct process memory allocator with maximum allocation
     * count.
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

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     * 
     * @return
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    [[nodiscard]] size_t acquisition_count() const noexcept;

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     * 
     * @return
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    [[nodiscard]] size_t max_acquisition_count() const noexcept;

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     * 
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     * 
     * @return
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    [[nodiscard]] memory::MemoryRegion acquire_chunk(units::Bytes size, size_t alignment);

    // ===================================================================================== //
    //      <> chevron::process::ProcessMemoryAllocator | [PRIVATE] ATTRIBUTES
    // ===================================================================================== //
private:
    AllocListHead allocHead_;          ///< Head of allocation tracking list
    std::atomic<size_t> chunkCount_;   ///< Count of chunks currently acquired
    size_t maxAllocs_;                 ///< Maximum OS memory acquisitions

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     * 
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     * 
     * @return
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    [[nodiscard]] void* allocate_aligned_memory(units::Bytes size, size_t alignment);

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     * 
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     * 
     * @return
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    void deallocate_aligned_memory(const memory::ChunkDescriptor& chunk) noexcept;
};

}

#endif // CHEVRON_LIB_PROCESS_MEMORY_ALLOCATOR_H_
