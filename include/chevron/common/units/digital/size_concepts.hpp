
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
 * Validates `Units` exposes a nested ByteRatio type that
 * is a `std::ratio`.
 */
template <typename Units>
concept has_byte_ratio_specification = requires {
	requires traits::is_std_ratio_v<typename Units::ByteRatio>;
};

/*!
 * @brief
 * Validates conversion between provided units is lossless.
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
 * Validates `UnitsT` belongs to the IEC binary digital size
 * system.
 */
template <typename UnitsT>
concept binary_digital_size_units = requires {
	requires has_byte_ratio_specification<UnitsT>;
	requires traits::is_binary_size_system_v<UnitsT>;
};

/*!
 * @brief
 * Validates `UnitsT` belongs to the SI decimal digital size
 * system.
 */
template <typename UnitsT>
concept decimal_digital_size_units = requires {
	requires has_byte_ratio_specification<UnitsT>;
	requires traits::is_decimal_size_system_v<UnitsT>;
};

/*!
 * @brief
 * Validates `Units_A` and `Units_B` belong to the same digital
 * size system.
 * 
 * @note
 * This concept does **NOT** check whether the two units are equal.
 * It asks whether they share the same baseline unit magnitude. The
 * IEC binary size system is based on 1,024; the SI decimal size
 * system is based on 1,000.
 */
template <typename Units_A, typename Units_B>
concept matching_digital_size_systems = requires {
	requires (traits::is_binary_size_system_v<Units_A> && traits::is_binary_size_system_v<Units_B>)
        || (traits::is_decimal_size_system_v<Units_A> && traits::is_decimal_size_system_v<Units_B>);
};

/*!
 * @brief
 * Validates `UnitsT` exposes the `DigitalSize` accessor API.
 */
template <typename UnitsT>
concept digital_size_unit_api = requires(const UnitsT& u) {
	{ u.count() } -> std::same_as<typename UnitsT::ReprType>;
	{ u.bytes() } -> std::same_as<typename UnitsT::ReprType>;
	{ u.size_t_bytes() } -> std::same_as<size_t>;
};

/*!
 * @brief
 * Validates `UnitsT` exposes the `DigitalSize` arithmetic
 * operations API.
 */
template <typename UnitsT>
concept digital_size_unit_arithmetic = requires(UnitsT& u, typename UnitsT::ReprType i) {
	// Relational operators
	{ u < u } -> std::same_as<bool>;
	{ u > u } -> std::same_as<bool>;
	{ u == u } -> std::same_as<bool>;
	{ u <= u } -> std::same_as<bool>;
	{ u >= u } -> std::same_as<bool>;
	// NOTE: Heterogeneous relational operations not covered by the above

	// Arithmetic operators
	{ u + u } -> std::same_as<UnitsT>;
	{ u - u } -> std::same_as<UnitsT>;
	{ u % u } -> std::same_as<UnitsT>;
	{ u / u } -> std::same_as<double>;
	// NOTE: Heterogeneous arithmetic operations not covered by the above
	{ u * i } -> std::same_as<UnitsT>;
	{ u / i } -> std::same_as<UnitsT>;

	// Arithmetic assignment operators
	{ u += u } -> std::same_as<UnitsT&>;
	{ u -= u } -> std::same_as<UnitsT&>;
	{ u %= u } -> std::same_as<UnitsT&>;
	// NOTE: Heterogeneous assignment operations not covered by the above
	{ u *= i } -> std::same_as<UnitsT&>;
	{ u /= i } -> std::same_as<UnitsT&>;
};

/*!
 * @brief
 * Validates `UnitsT` participates in the `DigitalSize`
 * unit system.
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
