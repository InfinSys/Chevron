
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file chunk_descriptor.hpp
 *
 * @brief
 * Non-copyable chunk descriptor for OS allocation
 * tracking.
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 04-27-2026
 */

#ifndef CHEVRON_LIB_MEMORY_CHUNK_DESCRIPTOR_H_
#define CHEVRON_LIB_MEMORY_CHUNK_DESCRIPTOR_H_

#include "chevron/memory/description.hpp"

namespace chevron::memory
{

/*!
 * @brief
 * Description of an OS allocation.
 * 
 * @details
 * Records a chunk of memory acquired from the OS for
 * tracking and eventual cleanup. Lives exclusively
 * within allocator tracking structures and does not
 * travel elsewhere. This type enforces single-location
 * residency.
 */
class ChunkDescriptor : public MemoryDescription {
	// ===================================================================================== //
    //      <> chevron::memory::ChunkDescriptor | CONSTRUCTORS / DESTRUCTOR
    // ===================================================================================== //
public:
	/*!
	 * @brief
	 * Construct chunk descriptor with base address, alignment
	 * guarantee, and capacity.
	 */
	ChunkDescriptor(void* base, size_t alignment, units::Bytes size) noexcept;

	ChunkDescriptor(const ChunkDescriptor&) = delete;

	/*!
	 * @brief
	 * Default construct chunk descriptor.
	 */
	ChunkDescriptor() noexcept;

	virtual ~ChunkDescriptor() noexcept = default;

	// ===================================================================================== //
    //      <> chevron::memory::ChunkDescriptor | OPERATORS
    // ===================================================================================== //

	ChunkDescriptor& operator=(const ChunkDescriptor&) = delete;
};

}

#endif // CHEVRON_LIB_MEMORY_CHUNK_DESCRIPTOR_H_
