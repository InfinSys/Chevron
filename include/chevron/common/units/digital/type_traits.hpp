
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
  * @file type_traits.hpp
  *
  * @brief
  * Defines digital unit entity related type traits.
  *
  * @author
  * Jamon T. Bailey
  *
  * @date 04-17-2026
  */

#ifndef CHEVRON_LIB_HDR_DIGITAL_UNIT_TYPE_TRAITS_H_
#define CHEVRON_LIB_HDR_DIGITAL_UNIT_TYPE_TRAITS_H_

#include <type_traits>
#include <ratio>
#include "chevron/utility/bits/powers.hpp"

namespace chevron::units::traits
{

/* ------------------------------------------------------------------------------------- */
//      > is_std_ratio | TYPE TRAIT
/* ------------------------------------------------------------------------------------- */

/*!
 * @brief
 * Base specialization of STL `std::ratio` type validation.
 */
template <typename T>
struct is_std_ratio : std::false_type {};

/*!
 * @brief
 * Specialization for any STL `std::ratio` instantiation.
 */
template <std::intmax_t Numerator, std::intmax_t Denominator>
struct is_std_ratio<std::ratio<Numerator, Denominator>> : std::true_type {};

/*!
 * @brief
 * Convenience alias for STL `std::ratio` type validation.
 */
template <typename T>
constexpr bool is_std_ratio_v = is_std_ratio<T>::value;

/* ------------------------------------------------------------------------------------- */
//      > finer_ratio / coarser_ratio | TYPE TRAIT
/* ------------------------------------------------------------------------------------- */

/*!
 * @brief
 * Determine which of two ratios is smaller.
 */
template <typename Ratio_A, typename Ratio_B>
using finer_ratio_t = std::conditional_t<
	std::ratio_less_v<Ratio_A, Ratio_B>, Ratio_A, Ratio_B>;

/*!
 * @brief
 * Determine which of two ratios is larger.
 */
template <typename Ratio_A, typename Ratio_B>
using coarser_ratio_t = std::conditional_t<
	!std::ratio_less_v<Ratio_A, Ratio_B>, Ratio_A, Ratio_B>;

/* ------------------------------------------------------------------------------------- */
//      > is_lossless_ratio_conversion | TYPE TRAIT
/* ------------------------------------------------------------------------------------- */

/*!
 * @brief
 * Validates conversion between provided ratios is lossless.
 * 
 * @details
 * A conversion is lossless when the source ratio is a
 * whole-number multiple of the target ratio. This is verified
 * by dividing the two ratios and confirming the denominator
 * of the **reduced** result is 1.
 */
template <typename From_Ratio, typename To_Ratio>
constexpr bool is_lossless_ratio_conversion_v = std::ratio_divide<From_Ratio, To_Ratio>::den == 1;

/* ------------------------------------------------------------------------------------- */
//      > finer_size / coarser_size | TYPE TRAIT
/* ------------------------------------------------------------------------------------- */

/*!
 * @brief
 * Determine which of two digital size units is smaller.
 */
template <typename Units_A, typename Units_B>
using finer_size_t = std::conditional_t<
    std::ratio_less_v<typename Units_A::ByteRatio, typename Units_B::ByteRatio>, Units_A, Units_B>;

/*!
 * @brief
 * Determine which of two digital size units is larger.
 */
template <typename Units_A, typename Units_B>
using coarser_size_t = std::conditional_t<
    !std::ratio_less_v<typename Units_A::ByteRatio, typename Units_B::ByteRatio>, Units_A, Units_B>;

/* ------------------------------------------------------------------------------------- */
//      > is_binary_size_system / is_decimal_size_system | TYPE TRAIT
/* ------------------------------------------------------------------------------------- */

/*!
 * @brief
 * Validates that `UnitsT` belongs to the IEC binary digital
 * size system.
 */
template <typename UnitsT>
constexpr bool is_binary_size_system_v =
	requires { typename UnitsT::ByteRatio; }
    && requires { typename UnitsT::ReprType; }
    && (UnitsT::ByteRatio::den == 1)
	&& bits::isPowerOfTwo(
		static_cast<UnitsT::ReprType>(UnitsT::ByteRatio::num)
	);

/*!
 * @brief
 * Validates that `UnitsT` belongs to the SI decimal digital
 * size system.
 */
template <typename UnitsT>
constexpr bool is_decimal_size_system_v =
	requires { typename UnitsT::ByteRatio; }
    && requires { typename UnitsT::ReprType; }
    && (UnitsT::ByteRatio::den == 1)
	&& bits::isPowerOfTen(
		static_cast<UnitsT::ReprType>(UnitsT::ByteRatio::num)
	);

}

#endif // CHEVRON_LIB_HDR_DIGITAL_UNIT_TYPE_TRAITS_H_
