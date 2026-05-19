
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file proc_mem_pool.hpp
 *
 * @brief
 * Process-wide fixed-size block pool with lock-free
 * allocation.
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 04-14-2026
 */

#ifndef CHEVRON_LIB_PROCESS_MEMORY_POOL_H_
#define CHEVRON_LIB_PROCESS_MEMORY_POOL_H_

#include "chevron/process/memory/mem_pool_config.hpp"
#include "chevron/process/memory/proc_allocator.hpp"
#include "chevron/memory/atomic_free_list.hpp"
#include "chevron/memory/chunk_descriptor.hpp"
#include "chevron/memory/region.hpp"

namespace chevron::process
{

/*!
 * @brief
 * Process-wide fixed-size memory block pool.
 * 
 * @note
 * The API and internals of this class are thread-safe. All
 * allocation and deallocation operations are lock-free.
 *
 * @details
 * This class is the mechanical core of Chevron's memory
 * hierarchy. It manages a lock-free embedded free list
 * of fixed-size blocks, backed by chunks acquired through
 * a composed memory allocator. Downstream consumers receive
 * blocks from this pool and build their own allocation
 * strategies on top of them.
 * 
 * This class is not used directly by downstream consumers.
 * It is composed as a member of `MemoryCore`, which provides
 * the policy surface (*preallocation*, *acquire/release
 * interface*) on top of the pool's mechanical operations.
 */
class ProcessMemoryPool {
    // ===================================================================================== //
    //      <> chevron::process::ProcessMemoryPool | [PRIVATE] NESTED TYPES
    // ===================================================================================== //

    /*!
     * @brief
     * Thread-local memory cache.
     * 
     * @details
     * Each thread that interacts with the pool maintains one of
     * these in thread-local storage. Allocation requests are
     * served from this cache first to mitigate atomic contention
     * on the process-level shared free list. When the cache is
     * empty, a batch of blocks is pulled from the shared pool to
     * refill local memory. When the cache exceeds its capacity,
     * excess blocks are drained back to the shared pool for other
     * threads to use. On thread exit, the destructor returns all
     * remaining blocks to the shared list.
     */
    struct ThreadLocalMemoryCache {
        memory::FreeRegionNode* free_list_head;   ///< Head of thread-local free block chain
        size_t free_blocks;                       ///< Number of blocks currently free in local cache
        size_t cached_blocks;                     ///< Number of blocks in local cache possession
        size_t batch_size;                        ///< Batch size for next trip to shared memory pool
        ProcessMemoryPool* shared_pool;           ///< Process-level shared memory pool

        /*! @brief Thread-local memory cache destructor. */
        ~ThreadLocalMemoryCache() noexcept;

        /*!
         * @brief
         * Checks whether this cache has blocks available for
         * allocation.
         * 
         * @return
         * True if local free list is not empty
         */
        [[nodiscard]] bool has_memory_free() const noexcept;

        /*!
         * @brief
         * Determines how many blocks to cache from shared pool.
         *
         * @return
         * Desired number of blocks to locally cache
         */
        [[nodiscard]] size_t compute_growth_batch() const noexcept;

        /*!
         * @brief
         * Determines how many blocks to return to shared pool.
         *
         * @return
         * Number of blocks to return
         */
        [[nodiscard]] size_t compute_shrink_batch() const noexcept;

    private:
        /*!
         * @brief
         * Returns all thread local memory to shared pool.
         * 
         * @details
         * Walks the thread local free list to the tail and pushes
         * the entire chain back to the process-level shared pool.
         * This method is invoked by the `ThreadLocalMemoryCache`
         * destructor to return all thread local memory.
         */
        void drain_cache() noexcept;
    };

    /*!
     * @brief
     * State of memory expansion execution path.
     * 
     * @details
     * Binary state set that articulates the status of the process
     * memory pool's expansion via an OS-bound memory allocation.
     * It is either the case that the expansion execution path is
     * `IDLE` (no OS-bound allocation is currently occurring), or
     * it is `EXPANDING` (currently fetching more memory from the
     * OS).
     */
    enum ExpansionState : size_t { IDLE, EXPANDING };

    /*!
     * @brief
     * Atomic memory pool expansion state.
     * 
     * @details
     * This is used in a CAS-based gate that ensure only one thread
     * performs an OS memory acquisition, preventing redundant chunk
     * allocations that could consume significant system memory. The
     * gate does not necessarily intend to serialize the expansion
     * path, but rather prevent 'expansion races' in concurrent
     * environments.
     */
    using AtomicExpandState = std::atomic<ExpansionState>;

    using atomic_size_t = std::atomic<size_t>;

	// ===================================================================================== //
    //      <> chevron::process::ProcessMemoryPool | CONSTRUCTORS / DESTRUCTOR
    // ===================================================================================== //
public:
    /*!
     * @brief
     * Construct process memory pool with provided configuration.
     */
    explicit ProcessMemoryPool(const MemoryPoolConfig& config);

    ProcessMemoryPool(const ProcessMemoryPool&) = delete;

    ProcessMemoryPool(ProcessMemoryPool&&) = delete;

    ~ProcessMemoryPool() noexcept;

    // ===================================================================================== //
    //      <> chevron::process::ProcessMemoryPool | [PUBLIC] MEMBER METHODS
    // ===================================================================================== //

    /*!
     * @brief
     * Returns size of memory block distributed to consumers.
     * 
     * @return
     * Effective block size after alignment adjustment
     */
    [[nodiscard]] units::Bytes distribution_size() const noexcept;

    /*!
     * @brief
     * Returns size of individual OS memory allocation chunk.
     *
     * @return
     * Effective chunk size after adjustment
     */
    [[nodiscard]] units::Bytes acquisition_size() const noexcept;

    /*!
     * @brief
     * Returns alignment guarantee for distributed blocks.
     *
     * @return
     * Maximum alignment all blocks satisfy
     */
    [[nodiscard]] size_t alignment_guarantee() const noexcept;

    /*!
     * @brief
     * Returns number of chunks currently acquired from OS.
     *
     * @return
     * Total number of OS allocations
     */
    [[nodiscard]] size_t chunk_count() const noexcept;

    /*!
     * @brief
     * Returns maximum number of chunks acquirable from OS.
     *
     * @return
     * Maximum number of OS allocations
     */
    [[nodiscard]] size_t max_chunk_count() const noexcept;

    /*!
     * @brief
     * Returns total bytes currently acquired from OS.
     *
     * @return
     * Total bytes reserved from OS
     */
    [[nodiscard]] units::Bytes bytes_in_possession() const noexcept;

    /*!
     * @brief
     * Acquire single block of memory from process-level pool.
     *
     * @return
     * Allocated memory region
     */
    [[nodiscard]] memory::MemoryRegion allocate();

    /*!
     * @brief
     * Return block of memory to pool.
     *
     * @details
     * The block is pushed onto the calling thread's local cache.
     * If the cache exceeds its capacity, a batch of blocks is
     * drained back to the shared free list.
     */
    void deallocate(memory::MemoryRegion& block) noexcept;

    // ===================================================================================== //
    //      <> chevron::process::ProcessMemoryPool | OPERATORS
    // ===================================================================================== //

    ProcessMemoryPool& operator=(const ProcessMemoryPool&) = delete;

    ProcessMemoryPool& operator=(ProcessMemoryPool&&) = delete;

    // ===================================================================================== //
    //      <> chevron::process::ProcessMemoryPool | [PRIVATE] ATTRIBUTES
    // ===================================================================================== //
private:
    memory::AtomicFreeList free_list_;   ///< Embedded free list
    atomic_size_t bytes_acquired_;       ///< Total bytes currently occupied from OS
    size_t blocks_per_chunk_;            ///< Number of blocks carved from each chunk
    MemoryPoolConfig config_;            ///< Process memory pool configuration 
    ProcessMemoryAllocator allocator_;   ///< Process memory allocator
    AtomicExpandState expansion_state_;  ///< Current state of memory expansion execution

    // ===================================================================================== //
    //      <> chevron::process::ProcessMemoryPool | [PRIVATE] MEMBER METHODS
    // ===================================================================================== //

    // -------------------------------------------------------------------------------------
    //      > chevron::process::ProcessMemoryPool | Constructor Helpers

    /*!
     * @brief
     * Adjusts block size for alignment and minimum size constraints.
     */
    void compute_effective_block_geometry();

    /*!
     * @brief
     * Adjusts chunk size to a clean multiple of block size.
     */
    void compute_effective_chunk_geometry();

    /*!
     * @brief
     * Validates memory pool budget constraints.
     */
    void validate_budget_constraints();

    /*!
     * @brief
     * Reconstructs process memory allocator with effective geometry
     * values.
     * 
     * @details
     * Uses placement new to reconstruct the allocator in-place because
     * its construction parameters depend on effective geometry values
     * that are not available at initializer list time.
     */
    void reinit_memory_allocator();

    /*!
     * @brief
     * Verifies memory pool is lock-free on current hardware.
     * 
     * @details
     * Checks that all atomic members are lock-free and that the CPU
     * supports a 16-byte atomic CAS instruction for the pool's free
     * list. Throws immediately if any check fails to prevent the
     * pool from operating without its lock-free guarantees.
     */
    void is_lock_free_or_throw();
    
    // -------------------------------------------------------------------------------------
    //      > chevron::process::ProcessMemoryPool | Operations

    /*!
     * @brief
     * Retrieves calling thread's memory cache.
     * 
     * @return
     * Calling thread's local memory cache
     */
    [[nodiscard]] ThreadLocalMemoryCache& get_current_thread_cache() noexcept;

    /*!
     * @brief
     * Carves provided memory chunk into chain of blocks and integrate
     * it into free list.
     * 
     * @details
     * Walks the provided chunk from base to end in block-sized steps,
     * writing embedded free region nodes into each block to form a
     * singly-linked chain. The chain is then pushed onto the shared
     * free list in a single batch operation.
     */
    void carve_and_link(const memory::MemoryRegion& chunk);

    /*!
     * @brief
     * Return chain of memory blocks to pool.
     */
    void push_batch(memory::FreeRegionNode* head, memory::FreeRegionNode* tail);

    /*!
     * @brief
     * Retrieves specified number of memory blocks from free list.
     * 
     * @return
     * Chain of free memory blocks
     */
    [[nodiscard]] memory::FreeRegionNode* pop_batch(size_t batch_size);

    /*!
     * @brief
     * Acquires new memory chunk from OS and integrates it into
     * pool's shared free list.
     * 
     * @details
     * Conducts an OS allocation to refill process-level memory. This
     * method prevents multiple threads from expanding the process
     * memory pool simultaneously. Concurrent callers yeild until the
     * expansion owner completes the process. The other threads of
     * execution then return without touching the expansion path. At
     * that point, the process-level shared pool would be refilled and
     * ready for a second `batch_pop()` attempt. There would be no
     * reason for another thread to trigger another OS-level allocation
     * if the memory configuration is properly tailored to the downstream
     * project's needs. The first thread to arrive at the expansion
     * execution path is the expansion owner.
     */
    void expand_memory();
};

}

#endif // CHEVRON_LIB_PROCESS_MEMORY_POOL_H_
