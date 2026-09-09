
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file alignment.test.cpp
 *
 * @brief
 * Unit tests for the generic alignment utilities.
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 09-08-2026
 */

#include <cstddef>
#include <limits>
#include <stdexcept>
#include <gtest/gtest.h>

#include "chevron/utility/bits/alignment.hpp"

using namespace chevron::bits;

// ===================================================================================== //
//      <> AlignUp Test Suite
// ===================================================================================== //

TEST(AlignUp, ValueThrowsOnZeroAlignment)
{
    EXPECT_THROW(alignUp(size_t{42}, size_t{0}), std::invalid_argument);
}

TEST(AlignUp, ValueReturnsSameWhenAlreadyAligned)
{
    EXPECT_EQ(alignUp(size_t{0}, size_t{8}), size_t{0});
    EXPECT_EQ(alignUp(size_t{8}, size_t{8}), size_t{8});
    EXPECT_EQ(alignUp(size_t{16}, size_t{8}), size_t{16});
    EXPECT_EQ(alignUp(size_t{64}, size_t{8}), size_t{64});
    EXPECT_EQ(alignUp(size_t{0}, size_t{16}), size_t{0});
    EXPECT_EQ(alignUp(size_t{256}, size_t{32}), size_t{256});
    EXPECT_EQ(alignUp(size_t{512}, size_t{256}), size_t{512});
    EXPECT_EQ(alignUp(size_t{100}, size_t{10}), size_t{100});
    EXPECT_EQ(alignUp(size_t{414}, size_t{23}), size_t{414});
}

TEST(AlignUp, ValueUnchangedWhenAlignmentIsOne)
{
    EXPECT_EQ(alignUp(size_t{0}, size_t{1}), size_t{0});
    EXPECT_EQ(alignUp(size_t{1}, size_t{1}), size_t{1});
    EXPECT_EQ(alignUp(size_t{12345}, size_t{1}), size_t{12345});
    EXPECT_EQ(
        alignUp(std::numeric_limits<size_t>::max(), size_t{1}),
        std::numeric_limits<size_t>::max()
    );
}

TEST(AlignUp, ValueRoundsUpToNearestMultiple)
{
    EXPECT_EQ(alignUp(size_t{5}, size_t{8}), size_t{8});
    EXPECT_EQ(alignUp(size_t{7}, size_t{4}), size_t{8});
    EXPECT_EQ(alignUp(size_t{10}, size_t{8}), size_t{16});
    EXPECT_EQ(alignUp(size_t{100}, size_t{32}), size_t{128});
    EXPECT_EQ(alignUp(size_t{1000}, size_t{64}), size_t{1024});
    EXPECT_EQ(alignUp(size_t{1}, size_t{16}), size_t{16});
    EXPECT_EQ(alignUp(size_t{100}, size_t{30}), size_t{120});
}

TEST(AlignUp, PointerThrowsOnZeroAlignment)
{
    alignas(64) std::byte buffer[128];
    std::byte* const ptr = buffer;

    EXPECT_THROW(alignUp(ptr, size_t{0}), std::invalid_argument);
}

TEST(AlignUp, PointerReturnsSameWhenAlreadyAligned)
{
    alignas(64) std::byte buffer[128];
    std::byte* const ptr = buffer;

    EXPECT_EQ(alignUp(ptr, size_t{16}), ptr);
    EXPECT_EQ(alignUp(ptr, size_t{64}), ptr);
}

TEST(AlignUp, PointerRoundsUpToNearestMultiple)
{
    alignas(64) std::byte buffer[128];
    std::byte* const ptr = buffer;

    EXPECT_EQ(alignUp(ptr + 3, size_t{16}), ptr + 16);
    EXPECT_EQ(alignUp(ptr + 1, size_t{64}), ptr + 64);
    EXPECT_EQ(alignUp(ptr + 33, size_t{32}), ptr + 64);
}

// ===================================================================================== //
//      <> AlignDown Test Suite
// ===================================================================================== //

TEST(AlignDown, ValueThrowsOnZeroAlignment)
{
    EXPECT_THROW(alignDown(size_t{42}, size_t{0}), std::invalid_argument);
}

TEST(AlignDown, ValueReturnsSameWhenAlreadyAligned)
{
    EXPECT_EQ(alignDown(size_t{0}, size_t{8}), size_t{0});
    EXPECT_EQ(alignDown(size_t{8}, size_t{8}), size_t{8});
    EXPECT_EQ(alignDown(size_t{16}, size_t{8}), size_t{16});
    EXPECT_EQ(alignDown(size_t{100}, size_t{10}), size_t{100});
    EXPECT_EQ(alignDown(size_t{414}, size_t{23}), size_t{414});
}

TEST(AlignDown, ValueUnchangedWhenAlignmentIsOne)
{
    EXPECT_EQ(alignDown(size_t{0}, size_t{1}), size_t{0});
    EXPECT_EQ(alignDown(size_t{1}, size_t{1}), size_t{1});
    EXPECT_EQ(alignDown(size_t{12345}, size_t{1}), size_t{12345});
    EXPECT_EQ(
        alignDown(std::numeric_limits<size_t>::max(), size_t{1}),
        std::numeric_limits<size_t>::max()
    );
}

TEST(AlignDown, ValueRoundsDownToNearestMultiple)
{
    EXPECT_EQ(alignDown(size_t{5}, size_t{8}), size_t{0});
    EXPECT_EQ(alignDown(size_t{7}, size_t{4}), size_t{4});
    EXPECT_EQ(alignDown(size_t{10}, size_t{8}), size_t{8});
    EXPECT_EQ(alignDown(size_t{100}, size_t{32}), size_t{96});
    EXPECT_EQ(alignDown(size_t{1000}, size_t{64}), size_t{960});
    EXPECT_EQ(alignDown(size_t{1}, size_t{16}), size_t{0});
    EXPECT_EQ(alignDown(size_t{100}, size_t{30}), size_t{90});
}

TEST(AlignDown, PointerThrowsOnZeroAlignment)
{
    alignas(64) std::byte buffer[128];
    std::byte* const ptr = buffer;

    EXPECT_THROW(alignDown(ptr, size_t{0}), std::invalid_argument);
}

TEST(AlignDown, PointerReturnsSameWhenAlreadyAligned)
{
    alignas(64) std::byte buffer[128];
    std::byte* const ptr = buffer;

    EXPECT_EQ(alignDown(ptr, size_t{16}), ptr);
    EXPECT_EQ(alignDown(ptr + 16, size_t{16}), ptr + 16);
    EXPECT_EQ(alignDown(ptr + 64, size_t{64}), ptr + 64);
}

TEST(AlignDown, PointerRoundsDownToNearestMultiple)
{
    alignas(64) std::byte buffer[128];
    std::byte* const ptr = buffer;

    EXPECT_EQ(alignDown(ptr + 3, size_t{16}), ptr);
    EXPECT_EQ(alignDown(ptr + 35, size_t{16}), ptr + 32);
    EXPECT_EQ(alignDown(ptr + 33, size_t{32}), ptr + 32);
}
