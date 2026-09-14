
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file powers.test.cpp
 *
 * @brief
 * Unit tests for the power-of-two and power-of-ten utilities.
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 09-08-2026
 */

#include <cstddef>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <gtest/gtest.h>

#include "chevron/utility/bits/powers.hpp"

using namespace chevron::bits;

// ===================================================================================== //
//      <> IsPowerOfTwo Test Suite
// ===================================================================================== //

/*!
 * @brief
 * Verifies that `isPowerOfTwo` accepts any power of two.
 *
 * @details
 * The predicate reports true exactly when the value can be
 * written as two raised to some non-negative exponent. The
 * samples cover the range of representable powers, from the
 * smallest, which is one, through the larger powers and up to
 * the top of the type's range, so the positive half of the
 * contract is pinned at multiple magnitudes.
 *
 * Each sample must be reported as a power of two. Any wrong
 * evaluation violates the contract and fails the test.
 */
TEST(IsPowerOfTwo, ReturnsTrueForPowersOfTwo)
{
    // Smallest power-of-two
    EXPECT_TRUE(isPowerOfTwo(size_t{1}));
    // Small power-of-two
    EXPECT_TRUE(isPowerOfTwo(size_t{2}));
    EXPECT_TRUE(isPowerOfTwo(size_t{4}));
    // Larger power-of-two
    EXPECT_TRUE(isPowerOfTwo(size_t{1024}));
    // Top-of-range power-of-two
    EXPECT_TRUE(isPowerOfTwo(size_t{1} << 31));
    EXPECT_TRUE(isPowerOfTwo(size_t{1} << 63));
}

/*!
 * @brief
 * Verifies that `isPowerOfTwo` rejects values that are not
 * powers of two.
 *
 * @details
 * The negative half of the contract must hold for every value
 * that is not a power of two. The samples cover both parities
 * and values near the top of the type's range, so the rejection
 * is pinned for ordinary values and for the extremes where an
 * implementation that scans or counts bits can misbehave.
 *
 * Zero is the boundary the documented contract singles out. No
 * integer exponent raises two to zero, so the predicate must
 * answer false for it.
 *
 * Each sample must be reported as not a power of two. Any wrong
 * evaluation violates the contract and fails the test.
 */
TEST(IsPowerOfTwo, ReturnsFalseForNonPowersOfTwo)
{
    // Zero value
    EXPECT_FALSE(isPowerOfTwo(size_t{0}));
    // Odd value
    EXPECT_FALSE(isPowerOfTwo(size_t{3}));
    EXPECT_FALSE(isPowerOfTwo(size_t{5}));
    EXPECT_FALSE(isPowerOfTwo(size_t{7}));
    EXPECT_FALSE(isPowerOfTwo(size_t{9}));
    // Even value
    EXPECT_FALSE(isPowerOfTwo(size_t{6}));
    EXPECT_FALSE(isPowerOfTwo(size_t{10}));
    EXPECT_FALSE(isPowerOfTwo(size_t{1000}));
    // Top-of-range value
    EXPECT_FALSE(isPowerOfTwo(std::numeric_limits<size_t>::max()));
    EXPECT_FALSE(isPowerOfTwo(std::numeric_limits<size_t>::max() - 1));
}

/*!
 * @brief
 * Verifies that `isPowerOfTwo` behaves correctly at the
 * boundaries of narrower unsigned types.
 *
 * @details
 * The predicate is a template over unsigned integral types,
 * and each instantiation answers for its own representable
 * range. The samples verify the predicate at widths narrower
 * than the platform's natural type, where the upper reach
 * differs and the template must still answer correctly for
 * each width.
 *
 * Each sampled width carries both halves of the contract at
 * its own boundary. The largest power of two the width can
 * hold must be accepted, and the value just past that power
 * must be rejected. Zero is rejected as well, since it is the
 * boundary every width shares, and the smallest power of two
 * is accepted at the narrowest width, so the template is
 * pinned at both ends of its range.
 *
 * Each sample must answer as the contract states. Any wrong
 * evaluation violates the contract and fails the test.
 */
TEST(IsPowerOfTwo, CorrectEvaluationAcrossUnsignedWidths)
{
    // 8-bit width/power-of-two
    EXPECT_TRUE(isPowerOfTwo(uint8_t{1}));
    EXPECT_TRUE(isPowerOfTwo(uint8_t{128}));
    // 8-bit width/non-power-of-two
    EXPECT_FALSE(isPowerOfTwo(uint8_t{0}));
    EXPECT_FALSE(isPowerOfTwo(uint8_t{129}));
    // 32-bit width/power-of-two
    EXPECT_TRUE(isPowerOfTwo(uint32_t{1} << 31));
    // 32-bit width/non-power-of-two
    EXPECT_FALSE(isPowerOfTwo(uint32_t{0}));
    EXPECT_FALSE(isPowerOfTwo(uint32_t{2147483649}));
}

// ===================================================================================== //
//      <> IsPowerOfTen Test Suite
// ===================================================================================== //

/*!
 * @brief
 * Verifies that `isPowerOfTen` accepts any power of ten.
 *
 * @details
 * The predicate reports true exactly when the value can be
 * written as ten raised to some non-negative exponent. The
 * samples cover the representable powers from the smallest,
 * which is one, up to the largest power of ten the type can
 * hold, so the contract is pinned across the range.
 *
 * Each sample must be reported as a power of ten. Any wrong
 * evaluation violates the contract and fails the test.
 */
TEST(IsPowerOfTen, ReturnsTrueForPowersOfTen)
{
    // Smallest power-of-ten
    EXPECT_TRUE(isPowerOfTen(size_t{1}));
    // Small power-of-ten
    EXPECT_TRUE(isPowerOfTen(size_t{10}));
    EXPECT_TRUE(isPowerOfTen(size_t{100}));
    // Larger power-of-ten
    EXPECT_TRUE(isPowerOfTen(size_t{1000}));
    EXPECT_TRUE(isPowerOfTen(size_t{1000000}));
    // Top-of-range power-of-ten
    EXPECT_TRUE(isPowerOfTen(size_t{10000000000000000000ULL}));
}

/*!
 * @brief
 * Verifies that `isPowerOfTen` rejects values that are not
 * powers of ten.
 *
 * @details
 * The negative half of the contract must hold for every value
 * that is not a power of ten. The samples cover odd values,
 * values that sit beside a power of ten, and the top of the
 * type's range, so the rejection is pinned for ordinary values
 * and for the near-miss cases where a repeated-division
 * implementation can slip.
 *
 * Zero is the boundary the documented contract singles out. No
 * integer exponent raises ten to zero, so the predicate must
 * answer false for it.
 *
 * Each sample must be reported as not a power of ten. Any wrong
 * evaluation violates the contract and fails the test.
 */
TEST(IsPowerOfTen, ReturnsFalseForNonPowersOfTen)
{
    // Zero value
    EXPECT_FALSE(isPowerOfTen(size_t{0}));
    // Odd value
    EXPECT_FALSE(isPowerOfTen(size_t{3}));
    EXPECT_FALSE(isPowerOfTen(size_t{5}));
    EXPECT_FALSE(isPowerOfTen(size_t{7}));
    EXPECT_FALSE(isPowerOfTen(size_t{1001}));
    EXPECT_FALSE(isPowerOfTen(size_t{1000001}));
    // Even value
    EXPECT_FALSE(isPowerOfTen(size_t{2}));
    EXPECT_FALSE(isPowerOfTen(size_t{20}));
    EXPECT_FALSE(isPowerOfTen(size_t{1100}));
    // Top-of-range value
    EXPECT_FALSE(isPowerOfTen(std::numeric_limits<size_t>::max()));
}

/*!
 * @brief
 * Verifies that `isPowerOfTen` behaves correctly at the
 * boundaries of narrower unsigned types.
 *
 * @details
 * The predicate is a template over unsigned integral types, and
 * each instantiation has its own top power of ten. The samples
 * exercise an eight-bit type at its largest power of ten and
 * just past it, and a thirty-two-bit type at its top power and
 * just past it, so the template is pinned at the width
 * extremes.
 *
 * Each sample must answer as the contract states. Any wrong
 * evaluation violates the contract and fails the test.
 */
TEST(IsPowerOfTen, CorrectEvaluationAcrossUnsignedWidths)
{
    // 8-bit width/power-of-ten
    EXPECT_TRUE(isPowerOfTen(std::uint8_t{1}));
    EXPECT_TRUE(isPowerOfTen(std::uint8_t{100}));
    // 8-bit width/non-power-of-ten
    EXPECT_FALSE(isPowerOfTen(std::uint8_t{0}));
    EXPECT_FALSE(isPowerOfTen(std::uint8_t{101}));
    EXPECT_FALSE(isPowerOfTen(std::uint8_t{120}));
    // 32-bit width/power-of-ten
    EXPECT_TRUE(isPowerOfTen(std::uint32_t{1000000000}));
    // 32-bit width/non-power-of-ten
    EXPECT_FALSE(isPowerOfTen(std::uint32_t{0}));
    EXPECT_FALSE(isPowerOfTen(std::uint32_t{1000000001}));
}

// ===================================================================================== //
//      <> PowerOfTwoCeil Test Suite
// ===================================================================================== //

/*!
 * @brief
 * Verifies that `powerOfTwoCeil` returns a power of two
 * unchanged.
 *
 * @details
 * A value that is already a power of two is already its own
 * ceiling, so the operation must pass it through untouched. The
 * samples cover the range of powers, from the smallest up to
 * the top of the type's range, so the identity holds at any
 * magnitude an implementation must not disturb.
 *
 * Each sample must return the value unchanged. Any altered
 * result violates the contract and fails the test.
 */
TEST(PowerOfTwoCeil, ReturnsUnchangedForPowersOfTwo)
{
    // Smallest power-of-two
    EXPECT_EQ(powerOfTwoCeil(size_t{1}), size_t{1});
    // Small power-of-two
    EXPECT_EQ(powerOfTwoCeil(size_t{2}), size_t{2});
    EXPECT_EQ(powerOfTwoCeil(size_t{8}), size_t{8});
    // Larger power-of-two
    EXPECT_EQ(powerOfTwoCeil(size_t{1024}), size_t{1024});
    EXPECT_EQ(powerOfTwoCeil(size_t{1} << 40), size_t{1} << 40);
    // Top-of-range power-of-two
    EXPECT_EQ(powerOfTwoCeil(size_t{1} << 63), size_t{1} << 63);
}

/*!
 * @brief
 * Verifies that `powerOfTwoCeil` raises a value to the next
 * power of two above it.
 *
 * @details
 * A value that falls between two powers of two must be raised
 * to the power above it. The samples cover small values,
 * values that sit just beside a boundary, and a larger value,
 * so the rounding is pinned in every position a value can
 * occupy between powers.
 *
 * Zero is not a power of two, yet the operation still has a
 * defined answer for it. The smallest power of two is one, so
 * the ceiling of zero must be one. The convention gives zero a
 * ceiling rather than leaving the value undefined, and it is
 * the sample most likely to be answered wrong by an
 * implementation that passes zero through untouched.
 *
 * Each sample must return the next power of two above the
 * value. Any other result violates the contract and fails the
 * test.
 */
TEST(PowerOfTwoCeil, RoundsUpToNextPowerOfTwo)
{
    // Zero value
    EXPECT_EQ(powerOfTwoCeil(size_t{0}), size_t{1});
    // Odd value
    EXPECT_EQ(powerOfTwoCeil(size_t{3}), size_t{4});
    EXPECT_EQ(powerOfTwoCeil(size_t{5}), size_t{8});
    EXPECT_EQ(powerOfTwoCeil(size_t{9}), size_t{16});
    EXPECT_EQ(powerOfTwoCeil(size_t{63}), size_t{64});
    EXPECT_EQ(powerOfTwoCeil(size_t{65}), size_t{128});
    // Even value
    EXPECT_EQ(powerOfTwoCeil(size_t{192}), size_t{256});
    EXPECT_EQ(powerOfTwoCeil(size_t{1000}), size_t{1024});
    // Top-of-range value
    EXPECT_EQ(powerOfTwoCeil((size_t{1} << 62) + size_t{7}), size_t{1} << 63);
    EXPECT_EQ(powerOfTwoCeil((size_t{1} << 40) + size_t{124}), size_t{1} << 41);
}

/*!
 * @brief
 * Verifies that `powerOfTwoCeil` throws when the result cannot
 * be represented.
 *
 * @details
 * Rounding upward has a limit. When the next power-of-two above
 * the value would exceed the largest value the type can hold,
 * the operation cannot produce a result and must refuse with an
 * overflow error. The samples sit just past the largest
 * representable powers of two, where the next power no longer
 * fits.
 *
 * The samples must throw `std::overflow_error`. A normal return
 * violates the contract and fails the test.
 */
TEST(PowerOfTwoCeil, ThrowsWhenResultExceedsTypeRange)
{
    // 64-bit width
    EXPECT_THROW(
        powerOfTwoCeil((uint64_t{1} << 63) + size_t{1}),
        std::overflow_error
    );
    // 32-bit width
    EXPECT_THROW(
        powerOfTwoCeil(uint32_t{2147483649}), ///< (2^31)+1
        std::overflow_error
    );
    // 8-bit width
    EXPECT_THROW(
        powerOfTwoCeil(uint8_t{129}),
        std::overflow_error
    );
}

// ===================================================================================== //
//      <> PowerOfTwoFloor Test Suite
// ===================================================================================== //

/*!
 * @brief
 * Verifies that `powerOfTwoFloor` throws for zero.
 *
 * @details
 * Zero has no previous power of two. There is no non-negative
 * exponent that produces zero, so there is no floor to return,
 * and the operation must refuse with an overflow error rather
 * than invent an answer.
 *
 * The sample must throw `std::overflow_error`. A normal return
 * violates the contract and fails the test.
 */
TEST(PowerOfTwoFloor, ThrowsOnZero)
{
    EXPECT_THROW(powerOfTwoFloor(size_t{0}), std::overflow_error);
}

/*!
 * @brief
 * Verifies that `powerOfTwoFloor` returns a power of two
 * unchanged.
 *
 * @details
 * A value that is already a power of two is already its own
 * floor, so the operation must pass it through untouched. The
 * samples cover the range of powers, from the smallest up to
 * the top of the type's range, so the identity holds at any
 * magnitude.
 *
 * Each sample must return the value unchanged. Any altered
 * result violates the contract and fails the test.
 */
TEST(PowerOfTwoFloor, ReturnsUnchangedForPowersOfTwo)
{
    // Smallest power-of-two
    EXPECT_EQ(powerOfTwoFloor(size_t{1}), size_t{1});
    // Small power-of-two
    EXPECT_EQ(powerOfTwoFloor(size_t{2}), size_t{2});
    EXPECT_EQ(powerOfTwoFloor(size_t{8}), size_t{8});
    // Larger power-of-two
    EXPECT_EQ(powerOfTwoFloor(size_t{4096}), size_t{4096});
    // Top-of-range power-of-two
    EXPECT_EQ(powerOfTwoFloor(size_t{1} << 63), size_t{1} << 63);
}

/*!
 * @brief
 * Verifies that `powerOfTwoFloor` lowers a value to the
 * previous power of two.
 *
 * @details
 * A value that falls between two powers of two must be lowered
 * to the power below it. The samples cover small values,
 * values near a boundary, a larger value, and the maximum
 * value, whose floor is the largest representable power of two.
 *
 * Each sample must return the previous power of two below the
 * value. Any other result violates the contract and fails the
 * test.
 */
TEST(PowerOfTwoFloor, RoundsDownToPreviousPowerOfTwo)
{
    // Odd value
    EXPECT_EQ(powerOfTwoFloor(size_t{3}), size_t{2});
    EXPECT_EQ(powerOfTwoFloor(size_t{5}), size_t{4});
    EXPECT_EQ(powerOfTwoFloor(size_t{9}), size_t{8});
    EXPECT_EQ(powerOfTwoFloor(size_t{63}), size_t{32});
    EXPECT_EQ(powerOfTwoFloor(size_t{65}), size_t{64});
    // Even value
    EXPECT_EQ(powerOfTwoFloor(size_t{1000}), size_t{512});
    EXPECT_EQ(powerOfTwoFloor(size_t{8180}), size_t{4096});
    // Top-of-range value
    EXPECT_EQ(
        powerOfTwoFloor(std::numeric_limits<size_t>::max()),
        size_t{1} << 63
    );
}

// ===================================================================================== //
//      <> PowerOfTenCeil Test Suite
// ===================================================================================== //

/*!
 * @brief
 * Verifies that `powerOfTenCeil` returns a power of ten
 * unchanged.
 *
 * @details
 * A value that is already a power of ten is already its own
 * ceiling, so the operation must pass it through untouched. The
 * samples cover the range of powers, from the smallest up to
 * the largest power of ten the type can hold.
 *
 * Each sample must return the value unchanged. Any altered
 * result violates the contract and fails the test.
 */
TEST(PowerOfTenCeil, ReturnsUnchangedForPowersOfTen)
{
    // Smallest power-of-ten
    EXPECT_EQ(powerOfTenCeil(size_t{1}), size_t{1});
    // Small powers-of-ten
    EXPECT_EQ(powerOfTenCeil(size_t{10}), size_t{10});
    // Larger power-of-ten
    EXPECT_EQ(powerOfTenCeil(size_t{1000}), size_t{1000});
    // Top-of-range power-of-ten
    EXPECT_EQ(
        powerOfTenCeil(size_t{10000000000000000000ULL}),
        size_t{10000000000000000000ULL}
    );
}

/*!
 * @brief
 * Verifies that `powerOfTenCeil` raises a value to the next
 * power of ten above it.
 *
 * @details
 * A value that falls between two powers of ten must be raised
 * to the power above it. The samples cover small values,
 * values that sit just beside a boundary, and a larger value,
 * so the rounding is pinned in any position a value can
 * occupy between powers.
 *
 * Zero is not a power of ten, yet the operation still has a
 * defined answer for it. The smallest power of ten is one, so
 * the ceiling of zero must be one. The convention gives zero a
 * ceiling rather than leaving the value undefined, and it is
 * the sample most likely to be answered wrong by an
 * implementation that passes zero through untouched.
 *
 * Each sample must return the next power of ten above the
 * value. Any other result violates the contract and fails the
 * test.
 */
TEST(PowerOfTenCeil, RoundsUpToNextPowerOfTen)
{
    // Zero value
    EXPECT_EQ(powerOfTenCeil(size_t{0}), size_t{1});
    // Even value
    EXPECT_EQ(powerOfTenCeil(size_t{2}), size_t{10});
    EXPECT_EQ(powerOfTenCeil(size_t{4096}), size_t{10000});
    // Odd value
    EXPECT_EQ(powerOfTenCeil(size_t{5}), size_t{10});
    EXPECT_EQ(powerOfTenCeil(size_t{9}), size_t{10});
    EXPECT_EQ(powerOfTenCeil(size_t{11}), size_t{100});
    EXPECT_EQ(powerOfTenCeil(size_t{99}), size_t{100});
    EXPECT_EQ(powerOfTenCeil(size_t{101}), size_t{1000});
    EXPECT_EQ(powerOfTenCeil(size_t{12345}), size_t{100000});
    // Top-of-range value
    EXPECT_EQ(
        powerOfTenCeil(size_t{1} << 63),
        size_t{10000000000000000000ULL}
    );
}

/*!
 * @brief
 * Verifies that `powerOfTenCeil` throws when the result cannot
 * be represented.
 *
 * @details
 * Rounding upward has a limit. When the next power of ten above
 * the value would exceed the largest value the type can hold,
 * the operation cannot produce a result and must refuse with an
 * overflow error. The samples sit just past the largest
 * representable powers of ten, where the next power no longer
 * fits.
 *
 * The samples must throw `std::overflow_error`. A normal return
 * violates the contract and fails the test.
 */
TEST(PowerOfTenCeil, ThrowsWhenResultExceedsTypeRange)
{
    // 64-bit width
    EXPECT_THROW(
        powerOfTenCeil(uint64_t{10000000000000000000ULL} + 1),
        std::overflow_error
    );
    // 32-bit width
    EXPECT_THROW(
        powerOfTenCeil(uint32_t{1000000000UL} + 1),
        std::overflow_error
    );
    // 8-bit width
    EXPECT_THROW(
        powerOfTenCeil(uint8_t{101}),
        std::overflow_error
    );
}

// ===================================================================================== //
//      <> PowerOfTenFloor Test Suite
// ===================================================================================== //

/*!
 * @brief
 * Verifies that `powerOfTenFloor` throws for zero.
 *
 * @details
 * Zero has no previous power of ten. There is no non-negative
 * exponent that produces zero, so there is no floor to return,
 * and the operation must refuse with an overflow error rather
 * than invent an answer.
 *
 * The sample must throw `std::overflow_error`. A normal return
 * violates the contract and fails the test.
 */
TEST(PowerOfTenFloor, ThrowsOnZero)
{
    EXPECT_THROW(powerOfTenFloor(size_t{0}), std::overflow_error);
}

/*!
 * @brief
 * Verifies that `powerOfTenFloor` returns a power of ten
 * unchanged.
 *
 * @details
 * A value that is already a power of ten is already its own
 * floor, so the operation must pass it through untouched. The
 * samples cover the range of powers, from the smallest up to
 * the largest power of ten the type can hold.
 *
 * Each sample must return the value unchanged. Any altered
 * result violates the contract and fails the test.
 */
TEST(PowerOfTenFloor, ReturnsUnchangedForPowersOfTen)
{
    // Smallest power-of-ten
    EXPECT_EQ(powerOfTenFloor(size_t{1}), size_t{1});
    // Small power-of-ten
    EXPECT_EQ(powerOfTenFloor(size_t{10}), size_t{10});
    // Larger power-of-ten
    EXPECT_EQ(powerOfTenFloor(size_t{1000}), size_t{1000});
    // Top-of-range power-of-ten
    EXPECT_EQ(
        powerOfTenFloor(size_t{10000000000000000000ULL}),
        size_t{10000000000000000000ULL}
    );
}

/*!
 * @brief
 * Verifies that `powerOfTenFloor` lowers a value to the
 * previous power of ten.
 *
 * @details
 * A value that falls between two powers of ten must be lowered
 * to the power below it. The samples cover small values,
 * values near a boundary, a larger value, and the maximum
 * value, whose floor is the largest representable power of ten.
 *
 * Each sample must return the previous power of ten below the
 * value. Any other result violates the contract and fails the
 * test.
 */
TEST(PowerOfTenFloor, RoundsDownToPreviousPowerOfTen)
{
    // Even value
    EXPECT_EQ(powerOfTenFloor(size_t{2}), size_t{1});
    EXPECT_EQ(powerOfTenFloor(size_t{50}), size_t{10});
    // Odd value
    EXPECT_EQ(powerOfTenFloor(size_t{5}), size_t{1});
    EXPECT_EQ(powerOfTenFloor(size_t{11}), size_t{10});
    EXPECT_EQ(powerOfTenFloor(size_t{99}), size_t{10});
    EXPECT_EQ(powerOfTenFloor(size_t{101}), size_t{100});
    EXPECT_EQ(powerOfTenFloor(size_t{12345}), size_t{10000});
    // Top-of-range value
    EXPECT_EQ(
        powerOfTenFloor(std::numeric_limits<size_t>::max()),
        size_t{10000000000000000000ULL}
    );
}

// ===================================================================================== //
//      <> MaxPowerOfTwoExponent Test Suite
// ===================================================================================== //

/*!
 * @brief
 * Verifies that `max_power_of_two_exponent` reports the
 * largest representable exponent for each width.
 *
 * @details
 * The exponent constant is the largest power of two that a
 * given unsigned type can represent, derived from the type's
 * own digit count. The samples pin the constant at several
 * widths so the template produces the correct value for each
 * instantiation.
 *
 * Each instantiation must report the exponent that matches its
 * width. Any other result violates the contract and fails the
 * test.
 */
TEST(MaxPowerOfTwoExponent, EqualsDigitsMinusOne)
{
    // 8-bit width
    EXPECT_EQ(max_power_of_two_exponent<uint8_t>, uint8_t{7});
    // 16-bit width
    EXPECT_EQ(max_power_of_two_exponent<uint16_t>, uint16_t{15});
    // 32-bit width
    EXPECT_EQ(max_power_of_two_exponent<uint32_t>, uint32_t{31});
    // 64-bit width
    EXPECT_EQ(max_power_of_two_exponent<uint64_t>, uint64_t{63});
}

// ===================================================================================== //
//      <> MaxPowerOfTenExponent Test Suite
// ===================================================================================== //

/*!
 * @brief
 * Verifies that `max_power_of_ten_exponent` reports the
 * largest representable exponent for each width.
 *
 * @details
 * The exponent constant is the largest power of ten a given
 * unsigned type can hold. Unlike the power-of-two constant,
 * this one cannot be derived from the digit count directly, so
 * the samples pin the computed value at several widths to make
 * sure each instantiation settles on the correct exponent.
 *
 * Each instantiation must report the exponent that matches its
 * width. Any other result violates the contract and fails the
 * test.
 */
TEST(MaxPowerOfTenExponent, EqualsHighestRepresentableExponent)
{
    // 8-bit width
    EXPECT_EQ(max_power_of_ten_exponent<uint8_t>, uint8_t{2});    // 100
    // 16-bit width
    EXPECT_EQ(max_power_of_ten_exponent<uint16_t>, uint16_t{4});  // 10,000
    // 32-bit width
    EXPECT_EQ(max_power_of_ten_exponent<uint32_t>, uint32_t{9});  // 1,000,000,000
    // 64-bit width
    EXPECT_EQ(max_power_of_ten_exponent<uint64_t>, uint64_t{19}); // Ten quintillion
}
