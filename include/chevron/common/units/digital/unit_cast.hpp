
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file unit_cast.hpp
 *
 * @brief
 * Digital size quantity cast utility.
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 04-18-2026
 */

#ifndef CHEVRON_LIB_HDR_DIGITAL_SIZE_UNIT_CAST_H_
#define CHEVRON_LIB_HDR_DIGITAL_SIZE_UNIT_CAST_H_

#include <cstdint>
#include <ratio>

namespace chevron::units
{

/*!
 * @brief
 * TODO : INCOMPLETE DOCUMENTATION!!!
 */
template <typename To_Units>
struct DigitalSizeCast {
	/*!
	 * @brief
	 * TODO : INCOMPLETE DOCUMENTATION!!!
	 */
	template <typename From_Units>
	static constexpr To_Units cast(const From_Units units)
	{
		using SourceRatio = typename From_Units::ByteRatio;
		using TargetRatio = typename To_Units::ByteRatio;
		using Factor = std::ratio_divide<SourceRatio, TargetRatio>;
		return To_Units{ (units.count() * Factor::num) / Factor::den };
	}

	DigitalSizeCast() noexcept = delete;
	~DigitalSizeCast() noexcept = delete;
};

}

#endif // CHEVRON_LIB_HDR_DIGITAL_SIZE_UNIT_CAST_H_
