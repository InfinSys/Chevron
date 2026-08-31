
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file memory_core.hpp
 *
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 04-01-2026
 */

#ifndef CHEVRON_LIB_PROCESS_MEMORY_AUTHORITY_H_
#define CHEVRON_LIB_PROCESS_MEMORY_AUTHORITY_H_

#include "chevron/process/memory/proc_allocator.hpp"
#include "chevron/process/memory/proc_mem_pool.hpp"

namespace chevron::process
{

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 *
 * @details
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
class MemoryCore {
public:
	MemoryCore(const MemoryPoolConfig& poolConfig) noexcept;

	~MemoryCore() = default;

private:
	ProcessMemoryAllocator procAlloc_; ///< Process-wide memory allocator
	ProcessMemoryPool procMemPool_;    ///< Process-wide memory pool

	/*!
	 * @brief
	 * TODO: INCOMPLETE DOCUMENTATION!!!
	 * 
	 * @details
	 * TODO: INCOMPLETE DOCUMENTATION!!!
	 */
	void init_memory_hierarchy();
};

}

#endif // CHEVRON_LIB_PROCESS_MEMORY_AUTHORITY_H_
