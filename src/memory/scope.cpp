
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file scope.cpp
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 05-26-2026
 */

#include "chevron/memory/scope.hpp"

#define EARLY_MEMORY_SCOPE_ERROR \
    "MemoryScope: Memory scope cannot be established before " \
    "the application process."

using chevron::memory::MemoryScope;

// ===================================================================================== //
//      <> chevron::memory::MemoryScope | CONSTRUCTORS / DESTRUCTOR
// ===================================================================================== //

MemoryScope::MemoryScope(Config scope_config)
	: free_list_{},
	parent_scope_{nullptr},
	parent_free_list_{nullptr},
	config_{scope_config}
{
	validate_scope_configuration();
}

MemoryScope::MemoryScope(MemoryScope& parent, Config scope_config)
	: free_list_{},
	parent_scope_{&parent},
	parent_free_list_{&parent.free_list_},
	config_{scope_config}
{
	validate_scope_configuration();
}

MemoryScope::~MemoryScope() noexcept
{
	// TODO: Handle MemoryScope destructor...
}

// ===================================================================================== //
//      <> chevron::memory::MemoryScope | [PRIVATE] MEMBER METHODS
// ===================================================================================== //

void MemoryScope::validate_scope_configuration() const
{
	if (!root_acquire_)
		throw std::runtime_error{EARLY_MEMORY_SCOPE_ERROR};

	// TODO: INCOMPLETE IMPLEMENTATION!!!
	// Verify this is in valid state...
	// (there are two possible modes here)
}

// ===================================================================================== //
//      <> chevron::memory::MemoryScope | [PRIVATE] STATIC METHODS
// ===================================================================================== //

void MemoryScope::set_global_root_acquisition(
	const Callable<MemoryRegion, FuncArgs<>>& acquisition_call) noexcept
{
	if (root_acquire_) return;
	root_acquire_ = new RootAcquisition{acquisition_call};
	// Note: Store this in our memory bounds instead?
}
