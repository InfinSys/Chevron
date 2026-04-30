
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
     * Atomic OS allocation tracking list pointer.
     * 
     * @details
     * Points to the first node in the singly-linked list of
     * acquired memory chunks. Nodes are prepended via CAS on
     * acquisition and the list is walked sequentially at
     * destruction for cleanup. No tagged pointer is needed
     * because nodes are only ever pushed during runtime, never
     * removed. So the ABA precondition is not a concern in this
     * circumstance.
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
     * 
     * @throws std::invalid_argument
     * if maximum OS memory acquisitions is zero.
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
     * Returns number of memory chunks currently acquired from OS.
     * 
     * @return
     * Current count of successful memory acquisitions
     */
    [[nodiscard]] size_t acquisition_count() const noexcept;

    /*!
     * @brief
     * Returns maximum number of memory chunks that may be acquired.
     * 
     * @return
     * OS memory acquisition limit
     */
    [[nodiscard]] size_t max_acquisition_count() const noexcept;

    /*!
     * @brief
     * Acquires new memory chunk from OS and records it for cleanup
     * later.
     * 
     * @details
     * Atomically increments the chunk counter to reserve the
     * permission for a new chunk. If the budget is exceeded, rolls
     * back and throws. Otherwise, acquires aligned memory from the
     * OS, constructs a tracking node, and CAS-pushes it onto the
     * tracking list.
     * 
     * @return
     * Memory region structure describing the acquired chunk
     * 
     * @throws std::bad_alloc
     * if the budget ceiling has been reached or the OS refuses the
     * allocation.
     */
    [[nodiscard]] memory::MemoryRegion acquire_chunk(units::Bytes size, size_t alignment);

    // ===================================================================================== //
    //      <> chevron::process::ProcessMemoryAllocator | [PRIVATE] ATTRIBUTES
    // ===================================================================================== //
private:
    AllocListHead allocListHead_;      ///< Head of allocation tracking list
    std::atomic<size_t> chunkCount_;   ///< Count of chunks currently acquired
    size_t maxAllocs_;                 ///< Maximum OS memory acquisitions

    /*!
     * @brief
     * Platform-specific aligned memory acquisition from OS.
     * 
     * @details
     * Acquires a contiguous region of at least the requested size
     * with the specified alignment guarantee on its starting
     * address.
     * 
     * @return
     * Raw pointer to acquired region of memory
     * 
     * @throws std::bad_alloc
     * when the OS refuses the allocation.
     */
    [[nodiscard]] void* allocate_aligned_memory(units::Bytes size, size_t alignment);

    /*!
     * @brief
     * Platform-specific memory release back to OS.
     */
    void deallocate_aligned_memory(const memory::ChunkDescriptor& chunk) noexcept;
};

}

#endif // CHEVRON_LIB_PROCESS_MEMORY_ALLOCATOR_H_
