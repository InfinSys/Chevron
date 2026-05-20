
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file region.cpp
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 04-28-2026
 */

#include "chevron/memory/region.hpp"

// ===================================================================================== //
//      <> chevron::memory::MemoryRegion | CONSTRUCTORS / DESTRUCTOR
// ===================================================================================== //

chevron::memory::MemoryRegion::MemoryRegion(void* base, size_t alignment, units::Bytes size) noexcept
	: MemoryDescription{base, alignment, size}
{
	//
}
