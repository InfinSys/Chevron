
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file size_units.hpp
 *
 * @brief
 * Digital size unit type aliases.
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 04-19-2026
 */

#ifndef CHEVRON_LIB_HDR_DIGITAL_SIZE_UNIT_ALIASES_H_
#define CHEVRON_LIB_HDR_DIGITAL_SIZE_UNIT_ALIASES_H_

#include "chevron/common/units/digital/digital_size.hpp"
#include "chevron/utility/math/algebra.hpp"

namespace chevron::units
{

// ===================================================================================== //
//      <> chevron::units::DigitalSize | TYPE ALIASES
// ===================================================================================== //

/*!
 * @brief
 * Bytes.
 */
using Bytes = units::DigitalSize<1>;

/*!
 * @brief
 * Kilobyte.
 */
using KB = units::DigitalSize<units::STD_DIGITAL_UNIT_MAGNITUDE>;

/*!
 * @brief
 * Megabyte.
 */
using MB = units::DigitalSize<math::exponent<units::STD_DIGITAL_UNIT_MAGNITUDE, 2>>;

/*!
 * @brief
 * Gigabyte.
 */
using GB = units::DigitalSize<math::exponent<units::STD_DIGITAL_UNIT_MAGNITUDE, 3>>;

/*!
 * @brief
 * Terabyte.
 */
using TB = units::DigitalSize<math::exponent<units::STD_DIGITAL_UNIT_MAGNITUDE, 4>>;

/*!
 * @brief
 * Kilobyte (IEC Standard - Kibibyte).
 */
using KiB = units::DigitalSize<units::IEC_DIGITAL_UNIT_MAGNITUDE>;

/*!
 * @brief
 * Megabyte (IEC Standard - Mebibyte).
 */
using MiB = units::DigitalSize<math::exponent<units::IEC_DIGITAL_UNIT_MAGNITUDE, 2>>;

/*!
 * @brief
 * Gigabyte (IEC Standard - Gibibyte).
 */
using GiB = units::DigitalSize<math::exponent<units::IEC_DIGITAL_UNIT_MAGNITUDE, 3>>;

/*!
 * @brief
 * Terabyte (IEC Standard - Tebibyte).
 */
using TiB = units::DigitalSize<math::exponent<units::IEC_DIGITAL_UNIT_MAGNITUDE, 4>>;

}

#endif // CHEVRON_LIB_HDR_DIGITAL_SIZE_UNIT_ALIASES_H_
