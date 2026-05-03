
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
#include "chevron/memory/memory_defs.hpp"
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
     * TODO: INCOMPLETE DOCUMENTATION!!!
     * 
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    struct ThreadLocalMemoryCache {
        memory::FreeRegionNode* free_list_head;   ///< Head of thread-local free block chain
        size_t free_blocks;                       ///< Number of blocks currently free in local cache
        size_t cached_blocks;                     ///< Number of blocks in local cache possession
        size_t batch_size;                        ///< Batch size for next trip to shared memory pool
        ProcessMemoryPool* shared_pool;           ///< Process-level shared memory pool

        /*!
         * @brief
         * TODO: INCOMPLETE DOCUMENTATION!!!
         * 
         * @details
         * TODO: INCOMPLETE DOCUMENTATION!!!
         */
        bool has_memory_free() const noexcept;

        /*!
         * @brief
         * TODO: INCOMPLETE DOCUMENTATION!!!
         * 
         * @details
         * TODO: INCOMPLETE DOCUMENTATION!!!
         */
        size_t compute_growth_batch() const noexcept;

        /*!
         * @brief
         * TODO: INCOMPLETE DOCUMENTATION!!!
         * 
         * @details
         * TODO: INCOMPLETE DOCUMENTATION!!!
         */
        size_t compute_shrink_batch() const noexcept;

        /*!
         * @brief
         * TODO: INCOMPLETE DOCUMENTATION!!!
         * 
         * @details
         * TODO: INCOMPLETE DOCUMENTATION!!!
         */
        void drain_cache() noexcept;

        ~ThreadLocalMemoryCache() noexcept;
    };

    /*!
     * @brief
     * State of pool's memory expansion execution path.
     * 
     * @details
     * Used as a CAS-based gate to ensure only one thread
     * performs OS memory acquisition at a time, preventing
     * redundant chunk allocations that could consume
     * significant system memory.
     */
    enum ExpansionState : size_t { IDLE, EXPANDING };

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    using ExpandState = std::atomic<ExpansionState>;

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    using FreeListHead = memory::AtomicFreeListHead;

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
     * TODO: INCOMPLETE DOCUMENTATION!!!
     * 
     * @return
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    [[nodiscard]] units::Bytes distribution_size() const noexcept;

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     *
     * @return
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    [[nodiscard]] units::Bytes acquisition_size() const noexcept;

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     *
     * @return
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    [[nodiscard]] size_t alignment_guarantee() const noexcept;

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     *
     * @return
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    [[nodiscard]] size_t chunk_count() const noexcept;

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     *
     * @return
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    [[nodiscard]] size_t max_chunk_count() const noexcept;

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     *
     * @return
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    [[nodiscard]] units::Bytes bytes_in_possession() const noexcept;

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     *
     * @return
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    [[nodiscard]] memory::MemoryRegion allocate();

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     *
     * @return
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    [[nodiscard]] void deallocate(const memory::MemoryRegion& block) noexcept;

    // ===================================================================================== //
    //      <> chevron::process::ProcessMemoryPool | OPERATORS
    // ===================================================================================== //

    ProcessMemoryPool& operator=(const ProcessMemoryPool&) = delete;

    ProcessMemoryPool& operator=(ProcessMemoryPool&&) = delete;

    // ===================================================================================== //
    //      <> chevron::process::ProcessMemoryPool | [PRIVATE] ATTRIBUTES
    // ===================================================================================== //
private:
    FreeListHead free_list_head_;        ///< Head of the embedded free list (ABA-safe)
    atomic_size_t bytes_acquired_;       ///< Total bytes currently occupied from OS
    size_t blocks_per_chunk_;            ///< Number of blocks carved from each chunk
    MemoryPoolConfig config_;            ///< Process memory pool configuration
    ProcessMemoryAllocator allocator_;   ///< Process memory allocator
    ExpandState expansion_state_;        ///< Current state of memory expansion execution

    // ===================================================================================== //
    //      <> chevron::process::ProcessMemoryPool | [PRIVATE] MEMBER METHODS
    // ===================================================================================== //

    // -------------------------------------------------------------------------------------
    //      > chevron::process::ProcessMemoryPool | Constructor Helpers

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    void compute_effective_block_geometry();

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    void compute_effective_chunk_geometry();

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    void validate_budget_constraints();

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    void reinit_memory_allocator();

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    void init_thread_cache_configuration();
    
    // -------------------------------------------------------------------------------------
    //      > chevron::process::ProcessMemoryPool | Operations

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     * 
     * @return
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    ThreadLocalMemoryCache& get_current_thread_cache() noexcept;

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     * 
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    void carve_and_link(const memory::MemoryRegion& chunk);

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     * 
     * @details
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    void push_batch(memory::FreeRegionNode* head, memory::FreeRegionNode* tail);

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
    memory::FreeRegionNode* pop_batch(size_t batch_size);

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    void expand_memory();

    // ===================================================================================== //
    //      <> chevron::process::ProcessMemoryPool | COMPILE-TIME GUARANTEES
    // ===================================================================================== //

    // static_assert(
    //     std::atomic<utility::TaggedPointer<void>>::is_always_lock_free,
    //     "ProcessMemoryPool requires lock-free 16-byte atomics. "
    //     "The target platform does not support lock-free std::atomic<TaggedPointer>."
    // );

};

}

#endif // CHEVRON_LIB_PROCESS_MEMORY_POOL_H_
