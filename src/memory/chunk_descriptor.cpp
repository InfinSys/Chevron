
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file chunk_descriptor.cpp
 *
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 04-27-2026
 */

#include "chevron/memory/chunk_descriptor.hpp"

// ===================================================================================== //
//      <> chevron::memory::ChunkDescriptor | CONSTRUCTORS / DESTRUCTOR
// ===================================================================================== //

chevron::memory::ChunkDescriptor::ChunkDescriptor(void* base, size_t alignment, units::Bytes size) noexcept
	: MemoryDescription{base, alignment, size}
{
	//
}
