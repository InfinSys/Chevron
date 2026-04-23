
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
#include "chevron/common/units/digital/size_concepts.hpp"

namespace chevron::units
{

/*!
 * @brief
 * Performs unconditional conversions between DigitalSize units.
 * 
 * @important
 * Conversions that go from finer to coarser grained units can
 * experience truncation (information loss) if the finer unit is
 * not in multiples of the coarser unit (i.e. 1,024 Bytes -> KB
 * = 1 KB with 24 bytes discarded). Conversions are determined to
 * be lossy or lossless at the call site.
 * 
 * @details
 * Given a digital size quantity expressed in one unit, an equivalent
 * quantity is provided in another specified digital size unit. The
 * target units are specified explicitly and the source units are
 * deduced from the `cast()` argument.
 * 
 * The cast is unconditional regardless of the possibility of lossing
 * information in the process. Conversions that cannot be represented
 * exact in the target units are truncated toward zero. Callers of this
 * utility are expected to be aware of lossy conversions during cast
 * that go from finer units to coarser units.
 * 
 * Truncation also occurs when casting between binary and decimal based
 * digital size systems (i.e. casting 1 KiB to 1 KB). The two sizing
 * systems work with different unit magnitude baselines (KiB = 1024
 * bytes vs. KB = 1000 bytes). The additional 24 bytes present in the
 * binary KiB are effectively discarded in the process rendering the
 * operation lossy. In the reverse direction, 1 KB is truncated to
 * 0 KiB since the decimal magnitude baseline is short 24 bytes to a
 * full 1 KiB.
 */
template <typename To_Units>
struct DigitalSizeCast {
	/*!
	 * @brief
	 * Converts provided units to an equivalent value in specified
	 * units.
	 * 
	 * @return
	 * Converted digital size units
	 */
	template <typename From_Units>
	static constexpr To_Units cast(const From_Units units) noexcept
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
