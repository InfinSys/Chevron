
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file description.hpp
 *
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
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
 * TODO: INCOMPLETE DOCUMENTATION!!!
 * 
 * @details
 * TODO: INCOMPLETE DOCUMENTATION!!!
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
	 * TODO: INCOMPLETE DOCUMENTATION!!!
	 */
	[[nodiscard]] void* base() const noexcept;

	/*!
	 * @brief
	 * Returns address alignment guarantee.
	 *
	 * @return
	 * TODO: INCOMPLETE DOCUMENTATION!!!
	 */
	[[nodiscard]] size_t alignment() const noexcept;

	/*!
	 * @brief
	 * Returns size of memory segment in bytes.
	 *
	 * @return
	 * TODO: INCOMPLETE DOCUMENTATION!!!
	 */
	[[nodiscard]] units::Bytes size() const noexcept;

	/*!
	 * @brief
	 * Returns byte count of memory segment.
	 *
	 * @return
	 * TODO: INCOMPLETE DOCUMENTATION!!!
	 */
	[[nodiscard]] size_t length() const noexcept;

	/*!
	 * @brief
	 * TODO: INCOMPLETE DOCUMENTATION!!!
	 *
	 * @return
	 * TODO: INCOMPLETE DOCUMENTATION!!!
	 */
	[[nodiscard]] std::byte* begin() const noexcept;

	/*!
	 * @brief
	 * TODO: INCOMPLETE DOCUMENTATION!!!
	 *
	 * @return
	 * TODO: INCOMPLETE DOCUMENTATION!!!
	 */
	[[nodiscard]] std::byte* end() const noexcept;

	/*!
	 * @brief
	 * TODO: INCOMPLETE DOCUMENTATION!!!
	 *
	 * @return
	 * TODO: INCOMPLETE DOCUMENTATION!!!
	 */
	[[nodiscard]] bool containsAddress(const void* ptr) const noexcept;

	/*!
	 * @brief
	 * TODO: INCOMPLETE DOCUMENTATION!!!
	 *
	 * @return
	 * TODO: INCOMPLETE DOCUMENTATION!!!
	 */
	[[nodiscard]] bool isAlignedTo(const size_t alignment) const noexcept;

	/*!
	 * @brief
	 * TODO: INCOMPLETE DOCUMENTATION!!!
	 *
	 * @return
	 * TODO: INCOMPLETE DOCUMENTATION!!!
	 */
	void clear() noexcept;

	// ===================================================================================== //
	//      <> chevron::memory::MemoryDescription | [PRIVATE] ATTRIBUTES
    // ===================================================================================== //
private:
	void* baseAddress_;
	size_t alignment_;
	size_t capacity_;
};

}

#endif // CHEVRON_LIB_MEMORY_DESCRIPTION_INTERFACE_H_
