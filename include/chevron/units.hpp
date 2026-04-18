
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
  * @file units.hpp
  *
  * @brief
  * General include header for unit measure related facilities.
  *
  * @author
  * Jamon T. Bailey
  *
  * @date 04-18-2026
  */

#ifndef CHEVRON_LIB_HDR_EXPLICIT_UNITS_INCLUDE_H_
#define CHEVRON_LIB_HDR_EXPLICIT_UNITS_INCLUDE_H_

#include "chevron/common/units/digital/digital_size.hpp"
#include "chevron/common/units/digital/unit_cast.hpp"

namespace chevron
{

/*!
 * @brief
 * TODO : INCOMPLETE DOCUMENTATION!!!
 */
template <typename To_Units, typename From_Units>
constexpr To_Units size_cast(const From_Units& source) noexcept
{
	return units::DigitalSizeCast<To_Units>::cast(source);
}

}

#endif // CHEVRON_LIB_HDR_EXPLICIT_UNITS_INCLUDE_H_
