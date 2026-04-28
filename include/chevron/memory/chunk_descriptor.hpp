
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file chunk_descriptor.hpp
 *
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
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
 * TODO: INCOMPLETE DOCUMENTATION!!!
 * 
 * @details
 * TODO: INCOMPLETE DOCUMENTATION!!!
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

	virtual ~ChunkDescriptor() noexcept = default;

	// ===================================================================================== //
    //      <> chevron::memory::ChunkDescriptor | OPERATORS
    // ===================================================================================== //

	ChunkDescriptor& operator=(const ChunkDescriptor&) = delete;
};

}

#endif // CHEVRON_LIB_MEMORY_CHUNK_DESCRIPTOR_H_
