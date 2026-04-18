
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

namespace chevron::units::traits
{

template <typename T>
struct has_byte_ratio_spec {
	//
};

//constexpr bool has_byte_ratio_spec_v = false;

/*!
 * @brief
 * Verify conversion between provided ratios is lossless.
 */
template <typename From_Ratio, typename To_Ratio>
constexpr bool is_lossless_ratio_conversion_v = std::ratio_divide<From_Ratio, To_Ratio>::den == 1;

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

}

#endif // CHEVRON_LIB_HDR_DIGITAL_UNIT_TYPE_TRAITS_H_
