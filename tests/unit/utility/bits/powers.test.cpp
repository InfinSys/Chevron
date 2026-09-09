
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
#include <gtest/gtest.h>

#include "chevron/utility/bits/powers.hpp"

using namespace chevron::bits;

// ===================================================================================== //
//      <> IsPowerOfTwo Test Suite
// ===================================================================================== //

TEST(IsPowerOfTwo, ReturnsTrueForPowersOfTwo)
{
    EXPECT_TRUE(isPowerOfTwo(size_t{1}));
    EXPECT_TRUE(isPowerOfTwo(size_t{2}));
    EXPECT_TRUE(isPowerOfTwo(size_t{4}));
    EXPECT_TRUE(isPowerOfTwo(size_t{16}));
    EXPECT_TRUE(isPowerOfTwo(size_t{1024}));
    EXPECT_TRUE(isPowerOfTwo(size_t{1} << 31));
    EXPECT_TRUE(isPowerOfTwo(size_t{1} << 63));
}

TEST(IsPowerOfTwo, ReturnsFalseForNonPowersOfTwo)
{
    EXPECT_FALSE(isPowerOfTwo(size_t{3}));
    EXPECT_FALSE(isPowerOfTwo(size_t{5}));
    EXPECT_FALSE(isPowerOfTwo(size_t{6}));
    EXPECT_FALSE(isPowerOfTwo(size_t{7}));
    EXPECT_FALSE(isPowerOfTwo(size_t{9}));
    EXPECT_FALSE(isPowerOfTwo(size_t{1000}));
    EXPECT_FALSE(isPowerOfTwo(std::numeric_limits<size_t>::max()));
    EXPECT_FALSE(isPowerOfTwo(std::numeric_limits<size_t>::max() - 1));
}

TEST(IsPowerOfTwo, ReturnsFalseForZero)
{
    EXPECT_FALSE(isPowerOfTwo(size_t{0}));
    EXPECT_FALSE(isPowerOfTwo(std::uint8_t{0}));
    EXPECT_FALSE(isPowerOfTwo(std::uint32_t{0}));
}

TEST(IsPowerOfTwo, WorksAcrossUnsignedWidths)
{
    EXPECT_TRUE(isPowerOfTwo(std::uint8_t{1}));
    EXPECT_TRUE(isPowerOfTwo(std::uint8_t{128}));
    EXPECT_FALSE(isPowerOfTwo(std::uint8_t{129}));
    EXPECT_TRUE(isPowerOfTwo(std::uint32_t{1} << 31));
    EXPECT_FALSE(isPowerOfTwo(std::uint32_t{2147483649}));
}

// ===================================================================================== //
//      <> IsPowerOfTen Test Suite
// ===================================================================================== //

TEST(IsPowerOfTen, ReturnsTrueForPowersOfTen)
{
    EXPECT_TRUE(isPowerOfTen(size_t{1}));
    EXPECT_TRUE(isPowerOfTen(size_t{10}));
    EXPECT_TRUE(isPowerOfTen(size_t{100}));
    EXPECT_TRUE(isPowerOfTen(size_t{1000}));
    EXPECT_TRUE(isPowerOfTen(size_t{1000000}));
    EXPECT_TRUE(isPowerOfTen(size_t{1000000000000000000ULL}));
}

TEST(IsPowerOfTen, ReturnsFalseForNonPowersOfTen)
{
    EXPECT_FALSE(isPowerOfTen(size_t{0}));
    EXPECT_FALSE(isPowerOfTen(size_t{2}));
    EXPECT_FALSE(isPowerOfTen(size_t{3}));
    EXPECT_FALSE(isPowerOfTen(size_t{5}));
    EXPECT_FALSE(isPowerOfTen(size_t{20}));
    EXPECT_FALSE(isPowerOfTen(size_t{1001}));
    EXPECT_FALSE(isPowerOfTen(size_t{1100}));
    EXPECT_FALSE(isPowerOfTen(size_t{1000001}));
    EXPECT_FALSE(isPowerOfTen(std::numeric_limits<size_t>::max()));
}

// ===================================================================================== //
//      <> PowerOfTwoCeil Test Suite
// ===================================================================================== //

TEST(PowerOfTwoCeil, ReturnsOneForZero)
{
    EXPECT_EQ(powerOfTwoCeil(size_t{0}), size_t{1});
}

TEST(PowerOfTwoCeil, ReturnsSameForPowersOfTwo)
{
    EXPECT_EQ(powerOfTwoCeil(size_t{1}), size_t{1});
    EXPECT_EQ(powerOfTwoCeil(size_t{2}), size_t{2});
    EXPECT_EQ(powerOfTwoCeil(size_t{8}), size_t{8});
    EXPECT_EQ(powerOfTwoCeil(size_t{1024}), size_t{1024});
    EXPECT_EQ(powerOfTwoCeil(size_t{1} << 40), size_t{1} << 40);
}

TEST(PowerOfTwoCeil, RoundsUpToNextPowerOfTwo)
{
    EXPECT_EQ(powerOfTwoCeil(size_t{3}), size_t{4});
    EXPECT_EQ(powerOfTwoCeil(size_t{5}), size_t{8});
    EXPECT_EQ(powerOfTwoCeil(size_t{9}), size_t{16});
    EXPECT_EQ(powerOfTwoCeil(size_t{63}), size_t{64});
    EXPECT_EQ(powerOfTwoCeil(size_t{64}), size_t{64});
    EXPECT_EQ(powerOfTwoCeil(size_t{65}), size_t{128});
    EXPECT_EQ(powerOfTwoCeil(size_t{1000}), size_t{1024});
}

TEST(PowerOfTwoCeil, RoundsUpLargeRepresentableValues)
{
    EXPECT_EQ(powerOfTwoCeil((size_t{1} << 40) + size_t{123}), size_t{1} << 41);
    EXPECT_EQ(powerOfTwoCeil((size_t{1} << 62) + size_t{7}), size_t{1} << 63);
}

TEST(PowerOfTwoCeil, ThrowsWhenResultExceedsTypeRange)
{
    EXPECT_THROW(
        powerOfTwoCeil((size_t{1} << 63) + size_t{1}),
        std::overflow_error
    );
}

// ===================================================================================== //
//      <> PowerOfTwoFloor Test Suite
// ===================================================================================== //

TEST(PowerOfTwoFloor, ThrowsOnZero)
{
    EXPECT_THROW(powerOfTwoFloor(size_t{0}), std::overflow_error);
}

TEST(PowerOfTwoFloor, ReturnsSameForPowersOfTwo)
{
    EXPECT_EQ(powerOfTwoFloor(size_t{1}), size_t{1});
    EXPECT_EQ(powerOfTwoFloor(size_t{2}), size_t{2});
    EXPECT_EQ(powerOfTwoFloor(size_t{8}), size_t{8});
    EXPECT_EQ(powerOfTwoFloor(size_t{1024}), size_t{1024});
}

TEST(PowerOfTwoFloor, RoundsDownToPreviousPowerOfTwo)
{
    EXPECT_EQ(powerOfTwoFloor(size_t{3}), size_t{2});
    EXPECT_EQ(powerOfTwoFloor(size_t{5}), size_t{4});
    EXPECT_EQ(powerOfTwoFloor(size_t{9}), size_t{8});
    EXPECT_EQ(powerOfTwoFloor(size_t{63}), size_t{32});
    EXPECT_EQ(powerOfTwoFloor(size_t{65}), size_t{64});
    EXPECT_EQ(powerOfTwoFloor(size_t{1000}), size_t{512});
    EXPECT_EQ(
        powerOfTwoFloor(std::numeric_limits<size_t>::max()),
        size_t{1} << 63
    );
}

// ===================================================================================== //
//      <> PowerOfTenCeil Test Suite
// ===================================================================================== //

TEST(PowerOfTenCeil, ReturnsOneForZero)
{
    EXPECT_EQ(powerOfTenCeil(size_t{0}), size_t{1});
}

TEST(PowerOfTenCeil, ReturnsSameForPowersOfTen)
{
    EXPECT_EQ(powerOfTenCeil(size_t{1}), size_t{1});
    EXPECT_EQ(powerOfTenCeil(size_t{10}), size_t{10});
    EXPECT_EQ(powerOfTenCeil(size_t{1000}), size_t{1000});
    EXPECT_EQ(
        powerOfTenCeil(size_t{1000000000000000000ULL}),
        size_t{1000000000000000000ULL}
    );
}

TEST(PowerOfTenCeil, RoundsUpToNextPowerOfTen)
{
    EXPECT_EQ(powerOfTenCeil(size_t{2}), size_t{10});
    EXPECT_EQ(powerOfTenCeil(size_t{5}), size_t{10});
    EXPECT_EQ(powerOfTenCeil(size_t{11}), size_t{100});
    EXPECT_EQ(powerOfTenCeil(size_t{99}), size_t{100});
    EXPECT_EQ(powerOfTenCeil(size_t{101}), size_t{1000});
    EXPECT_EQ(powerOfTenCeil(size_t{12345}), size_t{100000});
}

TEST(PowerOfTenCeil, ThrowsWhenResultExceedsTypeRange)
{
    EXPECT_THROW(
        powerOfTenCeil(size_t{10000000000000000000ULL} + 1),
        std::overflow_error
    );
}

// ===================================================================================== //
//      <> PowerOfTenFloor Test Suite
// ===================================================================================== //

TEST(PowerOfTenFloor, ThrowsOnZero)
{
    EXPECT_THROW(powerOfTenFloor(size_t{0}), std::overflow_error);
}

TEST(PowerOfTenFloor, ReturnsSameForPowersOfTen)
{
    EXPECT_EQ(powerOfTenFloor(size_t{1}), size_t{1});
    EXPECT_EQ(powerOfTenFloor(size_t{10}), size_t{10});
    EXPECT_EQ(powerOfTenFloor(size_t{1000}), size_t{1000});
}

TEST(PowerOfTenFloor, RoundsDownToPreviousPowerOfTen)
{
    EXPECT_EQ(powerOfTenFloor(size_t{2}), size_t{1});
    EXPECT_EQ(powerOfTenFloor(size_t{5}), size_t{1});
    EXPECT_EQ(powerOfTenFloor(size_t{11}), size_t{10});
    EXPECT_EQ(powerOfTenFloor(size_t{99}), size_t{10});
    EXPECT_EQ(powerOfTenFloor(size_t{101}), size_t{100});
    EXPECT_EQ(powerOfTenFloor(size_t{12345}), size_t{10000});
    EXPECT_EQ(
        powerOfTenFloor(std::numeric_limits<size_t>::max()),
        size_t{10000000000000000000ULL}
    );
}

// ===================================================================================== //
//      <> MaxPowerOfTwoExponent Test Suite
// ===================================================================================== //

TEST(MaxPowerOfTwoExponent, EqualsDigitsMinusOne)
{
    EXPECT_EQ(max_power_of_two_exponent<std::uint8_t>, std::uint8_t{7});
    EXPECT_EQ(max_power_of_two_exponent<std::uint16_t>, std::uint16_t{15});
    EXPECT_EQ(max_power_of_two_exponent<std::uint32_t>, std::uint32_t{31});
    EXPECT_EQ(max_power_of_two_exponent<size_t>, size_t{63});
}

// ===================================================================================== //
//      <> MaxPowerOfTenExponent Test Suite
// ===================================================================================== //

TEST(MaxPowerOfTenExponent, EqualsHighestRepresentableExponent)
{
    EXPECT_EQ(max_power_of_ten_exponent<std::uint8_t>, std::uint8_t{2});
    EXPECT_EQ(max_power_of_ten_exponent<std::uint16_t>, std::uint16_t{4});
    EXPECT_EQ(max_power_of_ten_exponent<std::uint32_t>, std::uint32_t{9});
    EXPECT_EQ(max_power_of_ten_exponent<size_t>, size_t{19});
}
