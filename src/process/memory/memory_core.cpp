
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file memory_core.cpp
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 04-01-2026
 */

#include "chevron/process/memory/memory_core.hpp"
#include "chevron/memory/scope.hpp"

using chevron::process::MemoryCore;
using chevron::process::MemoryPoolConfig;
using chevron::memory::MemoryRegion;
using chevron::memory::MemoryScope;

namespace {

size_t effectiveAllocationLimit(const MemoryPoolConfig& poolConfig) noexcept;

}

// ===================================================================================== //
//      <> chevron::process::MemoryCore | CONSTRUCTORS / DESTRUCTOR
// ===================================================================================== //

MemoryCore::MemoryCore(const MemoryPoolConfig& poolConfig) noexcept
	: procAlloc_{effectiveAllocationLimit(poolConfig)},
	procMemPool_{poolConfig}
{
	init_memory_hierarchy();
}

// ===================================================================================== //
//      <> chevron::process::MemoryCore | [PRIVATE] MEMBER METHODS
// ===================================================================================== //

void MemoryCore::init_memory_hierarchy()
{
	MemoryScope::set_global_root_acquisition(
		Callable{
			[&]() -> MemoryRegion {
				return procMemPool_.allocate_block(procAlloc_);
		    }
		}
	);
}

namespace {

// ===================================================================================== //
//      <> chevron::process::MemoryCore | INTERNAL FUNCTIONS
// ===================================================================================== //

/*!
 * @brief
 * Calculates maximum OS-bound allocations memory pool can conduct.
 * 
 * @return
 * Maximum memory pool OS allocation requests
 */
size_t maximumPoolAllocations(const MemoryPoolConfig& poolConfig) noexcept
{
	return static_cast<size_t>(poolConfig.budget_ceiling / poolConfig.chunk_size);
}

/*!
 * @brief
 * Calculate process-wide OS memory allocation limit.
 * 
 * @return
 * Maximum number of OS-bound memory allocation requests
 */
size_t effectiveAllocationLimit(const MemoryPoolConfig& poolConfig) noexcept
{
	return maximumPoolAllocations(poolConfig);
}

}
