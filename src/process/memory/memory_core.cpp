
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

using chevron::process::MemoryCore;
using chevron::process::MemoryPoolConfig;

static size_t effectiveAllocationLimit(const MemoryPoolConfig& poolConfig) noexcept;

// ===================================================================================== //
//      <> chevron::process::MemoryCore | CONSTRUCTORS / DESTRUCTOR
// ===================================================================================== //

MemoryCore::MemoryCore(const MemoryPoolConfig& poolConfig) noexcept
	: procAlloc_{effectiveAllocationLimit(poolConfig)},
	procMemPool_{poolConfig}
{
	//
}

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
static size_t maximumPoolAllocations(const MemoryPoolConfig& poolConfig) noexcept
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
static size_t effectiveAllocationLimit(const MemoryPoolConfig& poolConfig) noexcept
{
	return maximumPoolAllocations(poolConfig);
}
