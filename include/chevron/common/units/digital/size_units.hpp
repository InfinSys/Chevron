
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

/* ------------------------------------------------------------------------------------- */
//      > Decimal Digital Size Units
/* ------------------------------------------------------------------------------------- */

/*!
 * @brief
 * Kilobyte.
 */
using KB = units::DigitalSize<units::DECIMAL_DIGITAL_UNIT_MAGNITUDE>;

/*!
 * @brief
 * Megabyte.
 */
using MB = units::DigitalSize<math::exponent(units::DECIMAL_DIGITAL_UNIT_MAGNITUDE, 2)>;

/*!
 * @brief
 * Gigabyte.
 */
using GB = units::DigitalSize<math::exponent(units::DECIMAL_DIGITAL_UNIT_MAGNITUDE, 3)>;

/*!
 * @brief
 * Terabyte.
 */
using TB = units::DigitalSize<math::exponent(units::DECIMAL_DIGITAL_UNIT_MAGNITUDE, 4)>;

/* ------------------------------------------------------------------------------------- */
//      > Binary Digital Size Units
/* ------------------------------------------------------------------------------------- */

/*!
 * @brief
 * Kilobyte (IEC Standard - Kibibyte).
 * 
 * @note
 * This is a binary-based digital size unit that should not be
 * confused with its decimal counterpart.
 */
using KiB = units::DigitalSize<units::BINARY_DIGITAL_UNIT_MAGNITUDE>;

/*!
 * @brief
 * Megabyte (IEC Standard - Mebibyte).
 * 
 * @note
 * This is a binary-based digital size unit that should not be
 * confused with its decimal counterpart.
 */
using MiB = units::DigitalSize<math::exponent(units::BINARY_DIGITAL_UNIT_MAGNITUDE, 2)>;

/*!
 * @brief
 * Gigabyte (IEC Standard - Gibibyte).
 * 
 * @note
 * This is a binary-based digital size unit that should not be
 * confused with its decimal counterpart.
 */
using GiB = units::DigitalSize<math::exponent(units::BINARY_DIGITAL_UNIT_MAGNITUDE, 3)>;

/*!
 * @brief
 * Terabyte (IEC Standard - Tebibyte).
 * 
 * @note
 * This is a binary-based digital size unit that should not be
 * confused with its decimal counterpart.
 */
using TiB = units::DigitalSize<math::exponent(units::BINARY_DIGITAL_UNIT_MAGNITUDE, 4)>;

}

#endif // CHEVRON_LIB_HDR_DIGITAL_SIZE_UNIT_ALIASES_H_
