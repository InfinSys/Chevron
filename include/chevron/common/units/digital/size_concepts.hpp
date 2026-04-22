
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
  * @file size_concepts.hpp
  *
  * @brief
  * Defines digital unit entity related type concepts.
  *
  * @author
  * Jamon T. Bailey
  *
  * @date 04-20-2026
  */

#ifndef CHEVRON_LIB_HDR_DIGITAL_UNIT_TYPE_CONCEPTS_H_
#define CHEVRON_LIB_HDR_DIGITAL_UNIT_TYPE_CONCEPTS_H_

#include <concepts>
#include "chevron/common/units/digital/type_traits.hpp"

namespace chevron::units::concepts
{

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
template <typename Units>
concept has_byte_ratio_specification = requires {
	requires traits::is_std_ratio_v<typename Units::ByteRatio>;
};

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
template <typename From_Units, typename To_Units>
concept lossless_digital_size_conversion = requires {
	requires has_byte_ratio_specification<From_Units>
        && has_byte_ratio_specification<To_Units>;
	requires traits::is_lossless_ratio_conversion_v<
		typename From_Units::ByteRatio, typename To_Units::ByteRatio>;
};

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
template <typename UnitsT>
concept binary_digital_size_units = requires {
	requires traits::is_binary_size_system_v<UnitsT>;
};

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
template <typename UnitsT>
concept decimal_digital_size_units = requires {
	requires traits::is_decimal_size_system_v<UnitsT>;
};

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
template <typename Units_A, typename Units_B>
concept matching_digital_size_systems = requires {
	requires (traits::is_binary_size_system_v<Units_A> && traits::is_binary_size_system_v<Units_B>)
        || (traits::is_decimal_size_system_v<Units_A> && traits::is_decimal_size_system_v<Units_B>);
};

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
template <typename UnitsT>
concept digital_size_unit_api = requires(const UnitsT& u) {
	{ u.count() } -> std::same_as<typename UnitsT::ReprType>;
	{ u.bytes() } -> std::same_as<typename UnitsT::ReprType>;
	{ u.size_t_bytes() } -> std::same_as<size_t>;
};

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
template <typename UnitsT>
concept digital_size_unit_arithmetic = requires(UnitsT& u, typename UnitsT::ReprType i) {
	// Relational operators
	{ u < u } -> std::same_as<bool>;
	{ u > u } -> std::same_as<bool>;
	{ u == u } -> std::same_as<bool>;
	{ u <= u } -> std::same_as<bool>;
	{ u >= u } -> std::same_as<bool>;

	// Arithmetic operators
	{ u + u } -> std::same_as<UnitsT>;
	{ u - u } -> std::same_as<UnitsT>;
	// NOTE: Heterogeneous arithmetic results not covered by the above
	{ u * i } -> std::same_as<UnitsT>;
	{ u / i } -> std::same_as<UnitsT>;
	{ u / u } -> std::same_as<double>;

	// Arithmetic assignment operators
	{ u += u } -> std::same_as<UnitsT&>;
	{ u -= u } -> std::same_as<UnitsT&>;
	{ u *= i } -> std::same_as<UnitsT&>;
	{ u /= i } -> std::same_as<UnitsT&>;
};

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
template <typename UnitsT>
concept digital_size_units = requires {
	requires std::unsigned_integral<typename UnitsT::ReprType>;
	requires digital_size_unit_api<UnitsT>;
	requires digital_size_unit_arithmetic<UnitsT>;
	requires binary_digital_size_units<UnitsT> || decimal_digital_size_units<UnitsT>;
};

}

#endif // CHEVRON_LIB_HDR_DIGITAL_UNIT_TYPE_CONCEPTS_H_
