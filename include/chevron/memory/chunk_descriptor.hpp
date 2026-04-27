
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

namespace chevron::memory
{

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 * 
 * @details
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
class ChunkDescriptor {
public:
	/*! TODO: INCOMPLETE DOCUMENTATION!!! */
	ChunkDescriptor(void* base, size_t alignment, size_t capacity) noexcept;

	~ChunkDescriptor() noexcept = default;

private:
	void* base_;
	size_t alignment_;
	size_t capacity_;
};

}

#endif // CHEVRON_LIB_MEMORY_CHUNK_DESCRIPTOR_H_
