
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
 * Process-wide memory block pool configuration.
 *
 * @details
 * N/a
 */
struct CHEVRON_API MemoryPoolConfig {
	Bytes block_size;
	Bytes chunk_size;
	size_t block_alignment;
	Bytes budget_ceiling;
	size_t initial_thread_blocks;
	size_t max_thread_blocks;

	/*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
	 * 
	 * @return
	 * True if valid memory configuration
     */
	[[nodiscard]] bool isValid() const noexcept;

	/*!
	 * @brief
	 * TODO: INCOMPLETE DOCUMENTATION!!!
	 */
	void isValidOrThrow() const;

private:
	/*!
	 * @brief
	 * TODO: INCOMPLETE DOCUMENTATION!!!
	 */
	[[nodiscard]] bool validate(const char** reason) const noexcept;
};

}

#endif // CHEVRON_LIB_PROCESS_MEMORY_POOL_CONFIGURATION_H_
