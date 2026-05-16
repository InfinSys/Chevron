
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file region.hpp
 *
 * @brief
 * Distributable memory region handle.
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 04-27-2026
 */

#ifndef CHEVRON_LIB_MEMORY_REGION_H_
#define CHEVRON_LIB_MEMORY_REGION_H_

#include "chevron/memory/description.hpp"

namespace chevron::memory
{

/*!
 * @brief
 * Description of a contiguous memory region.
 *
 * @details
 * Lightweight handle used as the currency for memory transfer
 * across Chevron's memory hierarchy. This can be used for
 * describing whole blocks, subsets within blocks, or arbitrary
 * regions of managed memory. This type is freely copyable and
 * carries no ownership semantics.
 */
class CHEVRON_API MemoryRegion : public MemoryDescription {
	// ===================================================================================== //
    //      <> chevron::memory::MemoryRegion | CONSTRUCTORS / DESTRUCTOR
    // ===================================================================================== //
public:
	/*!
	 * @brief
	 * Construct memory handle with base address, alignment
	 * guarantee, and capacity.
	 */
	MemoryRegion(void* base, size_t alignment, units::Bytes size) noexcept;

	virtual ~MemoryRegion() noexcept = default;
};

}

#endif // CHEVRON_LIB_MEMORY_REGION_H_
