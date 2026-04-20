
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
#include "chevron/common/units/digital/size_units.hpp"
#include "chevron/common/units/digital/unit_cast.hpp"

namespace chevron
{

// ===================================================================================== //
//      <> DigitalSize | TYPE ALIASES
// ===================================================================================== //

using units::Bytes;  ///< Byte

using units::KiB;    ///< Kibibyte
using units::MiB;    ///< Mebibyte
using units::GiB;    ///< Gibibyte
using units::TiB;    ///< Tebibyte

using units::KB;     ///< Kilobyte
using units::MB;     ///< Megabyte
using units::GB;     ///< Gigabyte
using units::TB;     ///< Terabyte

/* ------------------------------------------------------------------------------------- */
//      > DigitalSize | Cast Operations
/* ------------------------------------------------------------------------------------- */

/*!
 * @brief
 * Cast digital size to another digital size.
 */
template <typename To_Units, typename From_Units>
constexpr To_Units size_cast(const From_Units& source) noexcept
{
	return units::DigitalSizeCast<To_Units>::cast(source);
}

// ===================================================================================== //
//      <> DigitalSize | USER-DEFINED LITERALS
// ===================================================================================== //

/*!
 * @brief
 * User-defined literals for digital size units.
 */
namespace digital_size::literals {

/*!
 * @brief
 * Byte integer literal.
 */
constexpr Bytes operator""_Bytes(unsigned long long value) noexcept
{
    return Bytes{static_cast<Bytes::ReprType>(value)};
}

/*!
 * @brief
 * Kilobyte integer literal.
 */
constexpr KB operator""_KB(unsigned long long value) noexcept
{
    return KB{static_cast<KB::ReprType>(value)};
}

/*!
 * @brief
 * Megabyte integer literal.
 */
constexpr MB operator""_MB(unsigned long long value) noexcept
{
    return MB{static_cast<MB::ReprType>(value)};
}

/*!
 * @brief
 * Gigabyte integer literal.
 */
constexpr GB operator""_GB(unsigned long long value) noexcept
{
    return GB{static_cast<GB::ReprType>(value)};
}

/*!
 * @brief
 * Terabyte integer literal.
 */
constexpr TB operator""_TB(unsigned long long value) noexcept
{
    return TB{static_cast<TB::ReprType>(value)};
}

/*!
 * @brief
 * IEC Standard kilobyte integer literal.
 */
constexpr KiB operator""_KiB(unsigned long long value) noexcept
{
    return KiB{static_cast<KiB::ReprType>(value)};
}

/*!
 * @brief
 * IEC Standard megabyte integer literal.
 */
constexpr MiB operator""_MiB(unsigned long long value) noexcept
{
    return MiB{static_cast<MiB::ReprType>(value)};
}

/*!
 * @brief
 * IEC Standard gigabyte integer literal.
 */
constexpr GiB operator""_GiB(unsigned long long value) noexcept
{
    return GiB{static_cast<GiB::ReprType>(value)};
}

/*!
 * @brief
 * IEC Standard terabyte integer literal.
 */
constexpr TiB operator""_TiB(unsigned long long value) noexcept
{
    return TiB{static_cast<TiB::ReprType>(value)};
}

}

}

#endif // CHEVRON_LIB_HDR_EXPLICIT_UNITS_INCLUDE_H_
