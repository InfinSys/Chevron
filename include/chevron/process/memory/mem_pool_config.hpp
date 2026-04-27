
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file mem_pool_config.hpp
 *
 * @brief
 * Declaration of process-wide block pool configuration.
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 04-14-2026
 */

#ifndef CHEVRON_LIB_PROCESS_MEMORY_POOL_CONFIGURATION_H_
#define CHEVRON_LIB_PROCESS_MEMORY_POOL_CONFIGURATION_H_

#include "chevron/units.hpp"
#include "chevron/common/export.h"

namespace chevron::process
{

/*!
 * @brief
 * Configuration parameters for process-wide memory pool.
 *
 * @details
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
struct CHEVRON_API MemoryPoolConfig {
	/*!
	 * @brief
	 * Size of individual distributed memory blocks.
	 * 
	 * @details
	 * Specifies the usable size of each block the pool distributes
	 * to downstream consumers. The pool may round this value up at
	 * construction to satisfy block alignment and internal minimum
	 * size requirements. This value does not reflect the final
	 * aligned block size. In this case, use the pools `blockSize()`
	 * method to query the effective block size after construction.
	 */
	Bytes block_size;

	/*!
	 * @brief
	 * Size of memory allocation requests to operating system.
	 * 
	 * @details
	 * The pool acquires memory from the OS in chunks of this size
	 * and carves each chunk into blocks. The pool may round this
	 * value up at construction to the nearest multiple of the
	 * effective block size so that no tail space is wasted within
	 * a chunk. Larger chunks reduce the frequency of OS acquisition
	 * at the cost of coarser memory commitment.
	 */
	Bytes chunk_size;

	/*!
	 * @brief
	 * Address alignment guarantee for all memory blocks.
	 * 
	 * @details
	 * Every block distributed by the pool will have a starting
	 * address that is a multiple of this value. This must be a
	 * power of two and has an effect on the effective block size.
	 * If the configured block size is not already a multiple of
	 * this alignment value, it will be rounded up to satisfy it.
	 */
	size_t block_alignment;

	/*!
	 * @brief
	 * Maximum memory the pool may hold from the operating system.
	 * 
	 * @details
	 * The pool acquires chunks lazily as demand requires until
	 * this ceiling. Once the ceiling is reached, further allocation
	 * requests that would require new chunks results in a thrown
	 * std::bad_alloc. This value must be at least as large as the
	 * effective chunk size after construction-time adjustments.
	 */
	Bytes budget_ceiling;

	/*!
	 * @brief
	 * Initial number of memory blocks in each thread's local memory
	 * cache.
	 * 
	 * @details
	 * When a thread first exhausts its local cache, it acquires this
	 * many blocks from the shared memory pool in a single run.
	 * Subsequent batch refills may increase local cache block count up
	 * to `max_thread_blocks`.
	 */
	size_t initial_thread_blocks;

	/*!
	 * @brief
	 * Maximum number of memory blocks a thread may locally cache.
	 * 
	 * @details
	 * This caps the growth of per-thread memory and prevents a long
	 * lived thread from consuming an excessive amount of memory from
	 * the shared memory pool.
	 */
	size_t max_thread_blocks;

	/*!
	 * @brief
	 * Maximum number of memory blocks a thread may request from the
	 * shared memory pool in a single run.
	 *
	 * @details
	 * This caps the function of growth on per-thread batch refills.
	 * While a thread is allowed to independently adjust the amount
	 * of blocks in a batch, it may never exceed this value. Limiting
	 * the batch size bounds the scope of each CAS operation against
	 * the shared free list and prevents a single refill from consuming
	 * a large portion of available blocks at once. This must be at
	 * least `initial_thread_blocks` and at most `max_thread_blocks`.
	 * Batch requests of sizes beyond this value are clamped down.
	 */
	size_t max_thread_batch_size;

	/*!
     * @brief
     * Validates memory configuration.
	 * 
	 * @return
	 * True if all configuration values are well-formed
     */
	[[nodiscard]] bool isValid() const noexcept;

	/*!
	 * @brief
	 * Attempts validation of memory configuration; throws if invalid.
	 * 
	 * @details
	 * In the event that a memory pool configuration is ill-formed,
	 * this method throws an exception rather than returning a boolean
	 * value.
	 */
	void isValidOrThrow() const;

private:
	/*!
	 * @brief
	 * Validates coherence of memory pool configuration.
	 * 
	 * @details
	 * Checks each memory pool configuration parameter in sequence. On
	 * the first violation an optional diagnostic string literal is
	 * written to the provided `reason` address. If all parameters are
	 * well-formed, the method returns true.
	 * 
	 * @param reason
	 * Address where diagnostic message is written on validation failure
	 * 
	 * @return
	 * True if valid configuration
	 */
	[[nodiscard]] bool validate(const char** reason) const noexcept;
};

}

#endif // CHEVRON_LIB_PROCESS_MEMORY_POOL_CONFIGURATION_H_
