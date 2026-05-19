
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file thread_mem_cache.cpp
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 05-02-2026
 */

#include "chevron/process/memory/proc_mem_pool.hpp"

using chevron::process::ProcessMemoryPool;
using chevron::memory::FreeRegionNode;

// ===================================================================================== //
//      <> ProcessMemoryPool::ThreadLocalMemoryCache | CONSTRUCTORS / DESTRUCTOR
// ===================================================================================== //

ProcessMemoryPool::ThreadLocalMemoryCache::~ThreadLocalMemoryCache() noexcept
{
	drain_cache();
}

// ===================================================================================== //
//      <> ProcessMemoryPool::ThreadLocalMemoryCache | [PUBLIC] MEMBER METHODS
// ===================================================================================== //

bool ProcessMemoryPool::ThreadLocalMemoryCache::has_memory_free() const noexcept
{
	return free_list_head != nullptr;
}

size_t ProcessMemoryPool::ThreadLocalMemoryCache::compute_growth_batch() const noexcept
{
	// TODO: INCOMPLETE IMPLEMENTATION!!!
	return 1;
}

size_t ProcessMemoryPool::ThreadLocalMemoryCache::compute_shrink_batch() const noexcept
{
	// TODO: INCOMPLETE IMPLEMENTATION!!!
	return 1;
}

void ProcessMemoryPool::ThreadLocalMemoryCache::drain_cache() noexcept
{
	if (free_list_head == nullptr) return;

	FreeRegionNode* cacheHead = free_list_head;
	FreeRegionNode* cacheTail = cacheHead;

	while (cacheTail->next != nullptr)
		cacheTail = static_cast<FreeRegionNode*>(cacheTail->next);

	shared_pool->push_batch(cacheHead, cacheTail);

	free_list_head = nullptr;
	cached_blocks = 0;
}
