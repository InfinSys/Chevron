
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
  * @file powers.hpp
  *
  * @brief
  * Predicates and rounding operations for power-of-two values.
  *
  * @author
  * Jamon T. Bailey
  *
  * @date 04-19-2026
  */

#ifndef CHEVRON_LIB_HDR_POWER_OF_TWO_UTILITIES_H_
#define CHEVRON_LIB_HDR_POWER_OF_TWO_UTILITIES_H_

#include <bit>
#include <limits>
#include <concepts>
#include <stdexcept>

namespace chevron::bits
{

// ===================================================================================== //
//      <> POWER-OF-TWO UTILITIES
// ===================================================================================== //

/*!
 * @brief
 * Determines maximum power of 2 exponent representable by
 * the provided unsigned integer type.
 */
template <std::unsigned_integral UIntT>
constexpr UIntT max_power_of_two_exponent = []() {
	return std::numeric_limits<UIntT>::digits - 1;
}();

/*!
 * @brief
 * Determines if provided unsigned integer is a power of 2.
 * 
 * @note
 * Note that `1` is considered a power of 2 because 2^0 is
 * equal to 1. Additionally, `0` is **NOT** considered a
 * power of 2 because no integer exponent produces a value
 * of zero with 2 as the base.
 * 
 * @return
 * True if provided value is a power of 2
 */
template <std::unsigned_integral UIntT>
constexpr bool isPowerOfTwo(UIntT value) noexcept {
	return std::has_single_bit(value);
}

/*!
 * @brief
 * Rounds provided value up to next power of 2.
 * 
 * @note
 * If the provided value is already a power of 2, it is
 * returned unchanged. Additionally, the smallest power
 * of 2 is 1, so `powerOfTwoCeil(0)` returns `1` by
 * convention.
 *
 * @return
 * Next power of 2
 */
template <std::unsigned_integral UIntT>
constexpr UIntT powerOfTwoCeil(UIntT value) {
	constexpr UIntT maxPower = max_power_of_two_exponent<UIntT>;

	if (value > maxPower) {
		throw std::overflow_error{
			"powerOfTwoCeil(): Value exceeds the maximum representable "
			"power of 2 for the target unsigned integral type."
		};
	}

	return std::bit_ceil(value);
}

/*!
 * @brief
 * Rounds provided value down to previous power of 2.
 * 
 * @note
 * If the provided value is already a power of 2, it is
 * returned unchanged. Additionally, the smallest power
 * of 2 is 1.
 *
 * @return
 * Previous power of 2
 */
template <std::unsigned_integral UIntT>
constexpr UIntT powerOfTwoFloor(UIntT value) {
	if (value == 0) {
		throw std::overflow_error{
			"powerOfTwoFloor(): Zero value has no previous power of 2."
		};
	}

	return std::bit_floor(value);
}

// ===================================================================================== //
//      <> POWER-OF-TEN UTILITIES
// ===================================================================================== //

/*!
 * @brief
 * Determines maximum power of 10 exponent representable
 * by the provided unsigned integer type.
 */
template <std::unsigned_integral UIntT>
constexpr UIntT max_power_of_ten_exponent = []() {
	constexpr UIntT uintMax = std::numeric_limits<UIntT>::max();
	UIntT exponent = 0;
	UIntT value = 1;

	while (value <= uintMax / 10) {
		value *= 10;
		++exponent;
	}

	return exponent;
}();

/*!
 * @brief
 * Determines if provided unsigned integer is a power of 10.
 * 
 * @note
 * Note that `1` is considered a power of 10 because 10^0 is
 * equal to 1. Additionally, `0` is **NOT** considered a
 * power of 10 because no integer exponent produces a value
 * of zero with 10 as the base.
 *
 * @return
 * True if provided value is a power of 10
 */
template <std::unsigned_integral UIntT>
constexpr bool isPowerOfTen(UIntT value) noexcept {
	if (value == 0 || value % 2 != 0)
		return false;

	while (value > 1) {
		const bool multipleOfTen = value % 10 != 0;

		if (multipleOfTen)
		    return false;
		else
		    value /= 10;
	}

	return true;
}

/*!
 * @brief
 * Rounds provided value up to next power of 10.
 * 
 * @note
 * If the provided value is already a power of 10, it is
 * returned unchanged. Additionally, the smallest power
 * of 10 is 1, so `powerOfTenCeil(0)` returns `1` by
 * convention.
 *
 * @return
 * Next power of 10
 */
template <std::unsigned_integral UIntT>
constexpr UIntT powerOfTenCeil(UIntT value) {
	if (value == 0)
		return UIntT{value};

	constexpr UIntT uintMax = std::numeric_limits<UIntT>::max();
	UIntT accumulator = 1;

	while (accumulator < value) {
		if (accumulator > uintMax / 10) {
			throw std::overflow_error{
				"powerOfTenCeil(): Value exceeds the maximum representable "
				"power of ten for the target unsigned integral type."
			};
		}

		accumulator *= 10;
	}

	return accumulator;
}

/*!
 * @brief
 * Rounds provided value down to previous power of 10.
 * 
 * @note
 * If the provided value is already a power of 10, it is
 * returned unchanged. Additionally, the smallest power
 * of 10 is 1.
 *
 * @return
 * Previous power of 10
 */
template <std::unsigned_integral UIntT>
constexpr UIntT powerOfTenFloor(UIntT value) {
	if (value == 0) {
		throw std::overflow_error{
			"powerOfTenFloor(): Zero value has no previous power of 10."
		};
	}

	constexpr UIntT uintMax = std::numeric_limits<UIntT>::max();
	UIntT accumulator = 1;

	while (accumulator <= value) {
		if (accumulator > uintMax / 10)
			return accumulator;

		const UIntT next = accumulator * 10;

		if (next > value)
			return accumulator;

		accumulator = next;
	}

	return accumulator;
}

}

#endif // CHEVRON_LIB_HDR_POWER_OF_TWO_UTILITIES_H_
