
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file description.hpp
 *
 * @brief
 * Base contiguous memory segment description type.
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 04-28-2026
 */

#ifndef CHEVRON_LIB_MEMORY_DESCRIPTION_INTERFACE_H_
#define CHEVRON_LIB_MEMORY_DESCRIPTION_INTERFACE_H_

#include <cstddef>
#include "chevron/common/units/digital/size_units.hpp"
#include "chevron/common/export.h"

namespace chevron::memory
{

/*!
 * @brief
 * Description of a contiguous segment of memory.
 * 
 * @details
 * This class exist as a common base for types that describe
 * a segment of memory. It holds a base address, alignment
 * guarantee, and capacity. It also provides shared query
 * operations for bounds checking, address containment, and
 * alignment verification.
 */
class CHEVRON_API MemoryDescription {
	// ===================================================================================== //
    //      <> chevron::memory::MemoryDescription | CONSTRUCTORS / DESTRUCTOR
    // ===================================================================================== //
protected:
	/*!
	 * @brief
	 * Construct memory description with base address, alignment
	 * guarantee, and capacity.
	 */
	MemoryDescription(void* base, size_t alignment, units::Bytes size) noexcept;

public:
	virtual ~MemoryDescription() noexcept = default;

	// ===================================================================================== //
	//      <> chevron::memory::MemoryDescription | [PUBLIC] MEMBER METHODS
	// ===================================================================================== //

	/*!
	 * @brief
	 * Returns starting address of memory segment.
	 *
	 * @return
	 * Memory segment base address
	 */
	[[nodiscard]] void* base() const noexcept;

	/*!
	 * @brief
	 * Returns address alignment guarantee.
	 *
	 * @return
	 * Address alignment
	 */
	[[nodiscard]] size_t alignment() const noexcept;

	/*!
	 * @brief
	 * Returns size of memory segment in bytes.
	 *
	 * @return
	 * Segment capacity as digital size
	 */
	[[nodiscard]] units::Bytes size() const noexcept;

	/*!
	 * @brief
	 * Returns byte count of memory segment.
	 *
	 * @return
	 * Segment capacity raw count
	 */
	[[nodiscard]] size_t length() const noexcept;

	/*!
	 * @brief
	 * Returns byte pointer to start of memory segment.
	 *
	 * @return
	 * Byte pointer to start of segment
	 */
	[[nodiscard]] std::byte* begin() const noexcept;

	/*!
	 * @brief
	 * Returns byte pointer one past end of memory segment.
	 *
	 * @return
	 * Byte pointer one past end of segment
	 */
	[[nodiscard]] std::byte* end() const noexcept;

	/*!
	 * @brief
	 * Checks whether given address falls within memory segment
	 * bounds.
	 *
	 * @return
	 * True if given address lies within this memory segment
	 */
	[[nodiscard]] bool containsAddress(const void* ptr) const noexcept;

	/*!
	 * @brief
	 * Checks whether base address satisfies a given alignment.
	 *
	 * @return
	 * True if base address is a multiple of given alignment
	 */
	[[nodiscard]] bool isAlignedTo(const size_t alignment) const noexcept;

	/*!
	 * @brief
	 * Invalidate this description of a memory segment.
	 *
	 * @details
	 * Sets base address, alignment, and capacity to default values.
	 * This method does not affect the memory being described, only
	 * the description itself is cleared. After returning from this
	 * call, the description no longer refers to any valid memory
	 * segment.
	 */
	void invalidate() noexcept;

	// ===================================================================================== //
	//      <> chevron::memory::MemoryDescription | [PRIVATE] ATTRIBUTES
    // ===================================================================================== //
private:
	void* baseAddress_;   ///< Base address of described memory region
	size_t alignment_;    ///< Alignment of described memory region
	size_t capacity_;     ///< Capacity of described memory region
};

}

#endif // CHEVRON_LIB_MEMORY_DESCRIPTION_INTERFACE_H_
