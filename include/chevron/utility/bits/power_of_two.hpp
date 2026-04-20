
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
  * @file power_of_two.hpp
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

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
template <std::unsigned_integral UIntT>
constexpr UIntT max_power_of_two = []() {
	return std::numeric_limits<UIntT>::digits - 1;
}();

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 * 
 * @return
 * True if provided value is a power of 2
 */
template <std::unsigned_integral UIntT>
constexpr bool is_power_of_two(UIntT value) noexcept {
	return std::has_single_bit(value);
}

/*!
 * @brief
 * Rounds provided value up to next power of 2.
 *
 * @return
 * Next power of 2
 */
template <std::unsigned_integral UIntT>
constexpr UIntT next_power_of_two(UIntT value) {
	constexpr UIntT maxPower = max_power_of_two<UIntT>;

	if (value > maxPower) {
		throw std::overflow_error{
			"next_power_of_two(): Value exceeds the maximum representable "
			"power of 2 for the target unsigned integral type."
		};
	}

	return std::bit_ceil(value);
}

/*!
 * @brief
 * Rounds provided value down to previous power of 2.
 *
 * @return
 * Previous power of 2
 */
template <std::unsigned_integral UIntT>
constexpr UIntT previous_power_of_two(UIntT value) {
	if (value == 0) {
		throw std::overflow_error{
			"previous_power_of_two(): Zero value has no previous power of 2."
		};
	}

	return std::bit_floor(value);
}

}

#endif // CHEVRON_LIB_HDR_POWER_OF_TWO_UTILITIES_H_
