
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
#include <array>
#include <stdexcept>
#include <gtest/gtest.h>

#include "chevron/utility/bits/alignment.hpp"

using namespace chevron::bits;

// ===================================================================================== //
//      <> AlignUp Test Suite
// ===================================================================================== //

/*!
 * @brief
 * Verifies that the scalar form of `alignUp` rejects a zero alignment
 * with a `std::invalid_argument` exception.
 *
 * @details
 * An alignment of zero leaves the rounding operation without a
 * meaningful result. There is no well-defined nearest multiple of
 * zero for the function to produce, so the input must be rejected
 * rather than silently answered. The rejection is pinned as
 * unconditional across the value dimension by sampling three
 * representative points: zero, an even value, and an odd value. The
 * zero case is the intersection where a zero value could be mistaken
 * for already aligned, while the even and odd cases show the
 * rejection holds regardless of parity or magnitude.
 *
 * Each sample must throw `std::invalid_argument`. A normal return
 * from any of the calls violates the contract and fails the test.
 */
TEST(AlignUp, ValueThrowsOnZeroAlignment)
{
    // Zero value
    EXPECT_THROW(alignUp(size_t{0}, size_t{0}), std::invalid_argument);
    // Even value
    EXPECT_THROW(alignUp(size_t{42}, size_t{0}), std::invalid_argument);
    // Odd/prime value
    EXPECT_THROW(alignUp(size_t{23}, size_t{0}), std::invalid_argument);
}

/*!
 * @brief
 * Verifies that the scalar form of `alignUp` returns the value
 * unchanged when it is already a multiple of the alignment.
 *
 * @details
 * Rounding upward only has meaning when the value is not already
 * at a multiple of the alignment. An already-aligned value is
 * already at the target, so the operation must be the identity and
 * pass it through unchanged. A function that rounds anyway pushes
 * every exact value one step too far.
 *
 * The samples cover the complete relationship between value and
 * alignment. A zero value is the only multiple of any alignment
 * that is smaller than the alignment itself, so it stands for the
 * entire lower range. The remaining samples cover a value equal to
 * the alignment and values greater than the alignment, and both of
 * those groups exercise power-of-two and non-power-of-two
 * alignments so the identity holds regardless of how the alignment
 * is shaped.
 *
 * Each pair must return the value unchanged. Any altered result
 * violates the contract and fails the test.
 */
TEST(AlignUp, ValueReturnsSameWhenAlreadyAligned)
{
    // Zero value
    EXPECT_EQ(alignUp(size_t{0}, size_t{8}), size_t{0});
    EXPECT_EQ(alignUp(size_t{0}, size_t{16}), size_t{0});
    // Value equal to alignment/power-of-two
    EXPECT_EQ(alignUp(size_t{8}, size_t{8}), size_t{8});
    EXPECT_EQ(alignUp(size_t{2048}, size_t{2048}), size_t{2048});
    // Value equal to alignment/non-power-of-two
    EXPECT_EQ(alignUp(size_t{3}, size_t{3}), size_t{3});
    // Value greater than alignment/power-of-two
    EXPECT_EQ(alignUp(size_t{16}, size_t{8}), size_t{16});
    EXPECT_EQ(alignUp(size_t{64}, size_t{8}), size_t{64});
    EXPECT_EQ(alignUp(size_t{256}, size_t{32}), size_t{256});
    EXPECT_EQ(alignUp(size_t{512}, size_t{256}), size_t{512});
    // Value greater than alignment/non-power-of-two
    EXPECT_EQ(alignUp(size_t{100}, size_t{10}), size_t{100});
    EXPECT_EQ(alignUp(size_t{414}, size_t{23}), size_t{414});
}

/*!
 * @brief
 * Verifies that the scalar form of `alignUp` returns the value
 * unchanged when the alignment is one.
 *
 * @details
 * An alignment of one is the identity alignment. Every value is
 * already a multiple of one, so there is never anything to round
 * and the operation must pass every value through unchanged. A
 * function that rounds anyway pushes every value one step too
 * far.
 *
 * The identity must hold at every magnitude. The samples cover a
 * zero value, the value equal to the alignment, an ordinary value
 * beyond it, and the maximum representable value. The maximum
 * sample targets implementations that blindly compute the next
 * multiple instead of recognizing the identity, which would wrap
 * around rather than return the value unchanged.
 *
 * Each sample must return the value unchanged. Any altered result
 * violates the contract and fails the test.
 */
TEST(AlignUp, ValueUnchangedWhenAlignmentIsOne)
{
    // Zero value
    EXPECT_EQ(alignUp(size_t{0}, size_t{1}), size_t{0});
    // Value equal to alignment
    EXPECT_EQ(alignUp(size_t{1}, size_t{1}), size_t{1});
    // Value greater than alignment
    EXPECT_EQ(alignUp(size_t{12345}, size_t{1}), size_t{12345});
    // Maximum representable value
    EXPECT_EQ(
        alignUp(std::numeric_limits<size_t>::max(), size_t{1}),
        std::numeric_limits<size_t>::max()
    );
}

/*!
 * @brief
 * Verifies that the scalar form of `alignUp` rounds a value up
 * to the next multiple of the alignment when it is not already
 * aligned.
 *
 * @details
 * Rounding upward is the core of the operation. A value that is
 * not already a multiple of the alignment must move up to the
 * next multiple above it, landing exactly there, neither below
 * nor one multiple past. This is the counterpart of the identity
 * behavior. An aligned value passes through unchanged, while a
 * misaligned value is brought to the boundary it was missing.
 *
 * The samples cover both sides of the relationship between value
 * and alignment. A value below the alignment rounds up to the
 * alignment itself, while a value above it rounds up to the next
 * multiple beyond. Each side is sampled with power-of-two and
 * non-power-of-two alignments, and the greater-than samples span
 * the distance past the boundary, from a value just one past an
 * exact multiple to values several multiples beyond.
 *
 * Each pair must round upward to the next multiple of the
 * alignment. Any other result violates the contract and fails
 * the test.
 */
TEST(AlignUp, ValueRoundsUpToNearestMultiple)
{
    // Value greater than alignment/power-of-two
    EXPECT_EQ(alignUp(size_t{7}, size_t{4}), size_t{8});
    EXPECT_EQ(alignUp(size_t{10}, size_t{8}), size_t{16});
    EXPECT_EQ(alignUp(size_t{100}, size_t{32}), size_t{128});
    EXPECT_EQ(alignUp(size_t{1000}, size_t{64}), size_t{1024});
    // Value greater than alignment/non-power-of-two
    EXPECT_EQ(alignUp(size_t{100}, size_t{30}), size_t{120});
    EXPECT_EQ(alignUp(size_t{47}, size_t{46}), size_t{92});
    // Value less than alignment/power-of-two
    EXPECT_EQ(alignUp(size_t{5}, size_t{8}), size_t{8});
    EXPECT_EQ(alignUp(size_t{1}, size_t{16}), size_t{16});
    // Value less than alignment/non-power-of-two
    EXPECT_EQ(alignUp(size_t{1}, size_t{30}), size_t{30});
}

/*!
 * @brief
 * Verifies that the pointer form of `alignUp` rejects a zero
 * alignment with a `std::invalid_argument` exception.
 *
 * @details
 * The pointer form carries the same contract as the scalar form.
 * A zero alignment is invalid regardless of which form receives
 * it, and the address carried by the pointer has no bearing on
 * the rejection. The pointer must be refused the same way a
 * value is refused, keeping the contract uniform across the
 * overload family.
 *
 * The rejection is pinned as unconditional across the address
 * dimension. The samples cover a null pointer, the address of an
 * aligned buffer, and a misaligned address. The null case is the
 * boundary where a null-pointer fast path could bypass the guard
 * entirely, while the aligned and misaligned cases show the
 * rejection holds no matter where the pointer points.
 *
 * Each sample must throw `std::invalid_argument`. A normal return
 * from any of the calls violates the contract and fails the test.
 */
TEST(AlignUp, PointerThrowsOnZeroAlignment)
{
    constexpr size_t ALIGNMENT = 64;
    constexpr size_t BUFFER_SIZE = 128;

    alignas(ALIGNMENT) std::array<std::byte, BUFFER_SIZE> buffer{};
    std::byte* const ptr = buffer.data();
    std::byte* const nullPtr = nullptr;

    // Null pointer address
    EXPECT_THROW(alignUp(nullPtr, size_t{0}), std::invalid_argument);
    // Aligned address
    EXPECT_THROW(alignUp(ptr, size_t{0}), std::invalid_argument);
    // Misaligned address
    EXPECT_THROW(alignUp(ptr + 3, size_t{0}), std::invalid_argument);
}

/*!
 * @brief
 * Verifies that the pointer form of `alignUp` returns an already
 * aligned address unchanged.
 *
 * @details
 * Rounding should never disturb an address that is already where
 * it belongs. A pointer sitting on a multiple of the alignment
 * has nothing to gain from rounding, so the function must return
 * it exactly as it was given. The null pointer belongs in that
 * same group. Aligning a pointer that holds no address must
 * leave it holding no address, so the identity holds for it as
 * naturally as it holds for a boundary-exact address.
 *
 * The samples exercise both members of that group. The null
 * samples confirm the identity for a pointer with no address at
 * all, across two alignment values, and the aligned samples
 * confirm it for a real boundary-exact address across the
 * alignment span the buffer can express, from the identity
 * alignment up to the buffer's own alignment. Together the two
 * categories show that a valid alignment leaves every
 * already-aligned pointer untouched, whatever its value.
 *
 * Each sample must return the address unchanged. Any altered
 * result violates the contract and fails the test.
 */
TEST(AlignUp, PointerReturnsSameWhenAlreadyAligned)
{
    constexpr size_t ALIGNMENT = 64;
    constexpr size_t BUFFER_SIZE = 128;

    alignas(ALIGNMENT) std::array<std::byte, BUFFER_SIZE> buffer{};
    std::byte* const ptr = buffer.data();
    std::byte* const nullPtr = nullptr;

    // Null pointer address
    EXPECT_EQ(alignUp(nullPtr, size_t{16}), nullPtr);
    EXPECT_EQ(alignUp(nullPtr, size_t{32}), nullPtr);
    // Aligned address
    EXPECT_EQ(alignUp(ptr, size_t{1}), ptr);
    EXPECT_EQ(alignUp(ptr, size_t{8}), ptr);
    EXPECT_EQ(alignUp(ptr, size_t{64}), ptr);
}

/*!
 * @brief
 * Verifies that the pointer form of `alignUp` rounds a misaligned
 * address up to the next multiple of the alignment.
 *
 * @details
 * A pointer that is not on a boundary still has a definite
 * destination. The rounding contract says it must move up to the
 * next multiple of the alignment and land precisely on it. The
 * samples stage that situation inside an aligned buffer, placing
 * each pointer at a known offset from the base and expecting the
 * result at the boundary that offset should reach.
 *
 * The offsets fall into the two groups a misaligned position can
 * belong to. A position short of the alignment's first boundary
 * climbs to that boundary itself, while a position that has
 * already passed a boundary continues to the one beyond. The 
 * alignment values come from the buffer's divisor family so every
 * expected address can be stated directly from the base.
 *
 * Each sample must round the address up to the next multiple of
 * the alignment. Any other result violates the contract and
 * fails the test.
 */
TEST(AlignUp, PointerRoundsUpToNearestMultiple)
{
    constexpr size_t ALIGNMENT = 64;
    constexpr size_t BUFFER_SIZE = 128;

    alignas(ALIGNMENT) std::array<std::byte, BUFFER_SIZE> buffer{};
    std::byte* const ptr = buffer.data();

    // Offset greater than alignment
    EXPECT_EQ(alignUp(ptr + 33, size_t{32}), ptr + 64);
    EXPECT_EQ(alignUp(ptr + 78, size_t{64}), ptr + 128);
    // Offset less than alignment
    EXPECT_EQ(alignUp(ptr + 3, size_t{16}), ptr + 16);
    EXPECT_EQ(alignUp(ptr + 1, size_t{64}), ptr + 64);
}

/*!
 * @brief
 * Verifies that the pointer form of `alignUp` rounds an address
 * without ever dereferencing the pointer.
 *
 * @details
 * The pointer form works with the address the pointer holds, not
 * with whatever sits behind it. The pointed-to object is never
 * part of the operation, and the function must not reach through
 * the pointer to read or write it. The test hands the function a
 * pointer whose address cannot be touched: if the implementation
 * stays on the arithmetic, the correct rounded address comes
 * back, and if it ever dereferences, the process faults and the
 * test fails on its own.
 *
 * A single sample is all the property warrants. Dereferencing is
 * a binary behavior, and no conditional reading of the contract
 * would make access acceptable for some alignments and not
 * others. One call with a valid alignment pins both halves of
 * the claim: no fault, and the address rounded correctly.
 *
 * The pointer must never be dereferenced. Any access to the
 * pointed-to memory violates the contract and fails the test.
 */
TEST(AlignUp, PointerRoundsWithoutDereference)
{
    std::byte* const illegalPtr = reinterpret_cast<std::byte*>(0x1);

    // Illegal address
    EXPECT_EQ(alignUp(illegalPtr, size_t{16}), illegalPtr + 15);
}

// ===================================================================================== //
//      <> AlignDown Test Suite
// ===================================================================================== //

/*!
 * @brief
 * Verifies that the scalar form of `alignDown` rejects a zero
 * alignment with a `std::invalid_argument` exception.
 *
 * @details
 * Rounding downward needs a boundary to land on, and a zero
 * alignment provides none. There is no multiple of zero to
 * descend to, so the function must refuse the request rather
 * than return an answer that has no meaning.
 *
 * The refusal is fixed across the value dimension by a sample
 * from each of its regions: zero, an even value, and an odd
 * value. The zero sample guards the case where a zero value is
 * mistaken for already aligned and answered instead of refused,
 * while the other two show the refusal holds whatever the
 * value's shape.
 *
 * Each sample must throw `std::invalid_argument`. A normal return
 * from any of the calls violates the contract and fails the test.
 */
TEST(AlignDown, ValueThrowsOnZeroAlignment)
{
    // Zero value
    EXPECT_THROW(alignDown(size_t{0}, size_t{0}), std::invalid_argument);
    // Even value
    EXPECT_THROW(alignDown(size_t{128}, size_t{0}), std::invalid_argument);
    // Odd/prime value
    EXPECT_THROW(alignDown(size_t{97}, size_t{0}), std::invalid_argument);
}

/*!
 * @brief
 * Verifies that the scalar form of `alignDown` returns the value
 * unchanged when it is already a multiple of the alignment.
 *
 * @details
 * Rounding downward has nothing to offer a value that is already
 * sitting on a boundary. The value is exactly where the contract
 * would put it, so the operation must leave it in place rather
 * than push it off the boundary it already holds.
 *
 * The samples cover every relationship a value can have with its
 * alignment. Zero is the only multiple that can fall below the
 * alignment, so it covers that entire range alone. A value equal
 * to the alignment and values greater than it make up the rest,
 * and both groups exercise power-of-two and non-power-of-two
 * alignments so the identity holds however the alignment is
 * shaped.
 *
 * Each pair must return the value unchanged. Any altered result
 * violates the contract and fails the test.
 */
TEST(AlignDown, ValueReturnsSameWhenAlreadyAligned)
{
    // Zero value
    EXPECT_EQ(alignDown(size_t{0}, size_t{8}), size_t{0});
    EXPECT_EQ(alignDown(size_t{0}, size_t{10}), size_t{0});
    // Value equal to alignment/power-of-two
    EXPECT_EQ(alignDown(size_t{16}, size_t{16}), size_t{16});
    EXPECT_EQ(alignDown(size_t{1024}, size_t{1024}), size_t{1024});
    // Value equal to alignment/non-power-of-two
    EXPECT_EQ(alignDown(size_t{5}, size_t{5}), size_t{5});
    // Value greater than alignment/power-of-two
    EXPECT_EQ(alignDown(size_t{16}, size_t{8}), size_t{16});
    EXPECT_EQ(alignDown(size_t{32}, size_t{8}), size_t{32});
    EXPECT_EQ(alignDown(size_t{96}, size_t{32}), size_t{96});
    EXPECT_EQ(alignDown(size_t{768}, size_t{256}), size_t{768});
    // Value greater than alignment/non-power-of-two
    EXPECT_EQ(alignDown(size_t{150}, size_t{50}), size_t{150});
    EXPECT_EQ(alignDown(size_t{336}, size_t{42}), size_t{336});
}

/*!
 * @brief
 * Verifies that the scalar form of `alignDown` returns the value
 * unchanged when the alignment is one.
 *
 * @details
 * An alignment of one leaves the operation with nothing to do.
 * Every value is already a multiple of one, so the function must
 * hand each value back exactly as it received it.
 *
 * The identity must hold at every magnitude. The samples cover
 * the smallest value the type can hold, the value equal to the
 * alignment, an ordinary value beyond it, and the maximum
 * representable value. The maximum sample guards the top of the
 * range, where an implementation that applies an adjustment
 * instead of recognizing the identity would disturb the value.
 *
 * Each sample must return the value unchanged. Any altered result
 * violates the contract and fails the test.
 */
TEST(AlignDown, ValueUnchangedWhenAlignmentIsOne)
{
    // Zero value
    EXPECT_EQ(alignDown(size_t{0}, size_t{1}), size_t{0});
    // Value equal to alignment
    EXPECT_EQ(alignDown(size_t{1}, size_t{1}), size_t{1});
    // Value greater than alignment
    EXPECT_EQ(alignDown(size_t{54321}, size_t{1}), size_t{54321});
    // Maximum representable value
    EXPECT_EQ(
        alignDown(std::numeric_limits<size_t>::max(), size_t{1}),
        std::numeric_limits<size_t>::max()
    );
}

/*!
 * @brief
 * Verifies that the scalar form of `alignDown` rounds a value
 * down to the nearest multiple of the alignment when it is not
 * already aligned.
 *
 * @details
 * Rounding downward sends a misaligned value to the nearest
 * multiple beneath it. The value must land there exactly, never
 * above and never past. A value below the alignment has only one
 * multiple beneath it, and that multiple is zero.
 *
 * The samples cover the two sides of a value's position, and
 * each side carries a case that a nearest-rounding implementation
 * would answer wrong. A value more than halfway past a multiple
 * tempts a nearest-rounder upward, but the contract demands the
 * multiple below. A value below the alignment, more than halfway
 * toward it, tempts the same implementation to climb to the
 * alignment when the only correct answer is zero. The ordinary
 * samples sit closer to the multiple below, where both readings
 * agree, so the test does not rely on adversarial points alone.
 *
 * Each pair must round downward to the nearest multiple of the
 * alignment. Any other result violates the contract and fails
 * the test.
 */
TEST(AlignDown, ValueRoundsDownToNearestMultiple)
{
    // Value greater than alignment/power-of-two
    EXPECT_EQ(alignDown(size_t{14}, size_t{8}), size_t{8});
    EXPECT_EQ(alignDown(size_t{36}, size_t{16}), size_t{32});
    EXPECT_EQ(alignDown(size_t{208}, size_t{64}), size_t{192});
    EXPECT_EQ(alignDown(size_t{3000}, size_t{512}), size_t{2560});
    // Value greater than alignment/non-power-of-two
    EXPECT_EQ(alignDown(size_t{47}, size_t{30}), size_t{30});
    EXPECT_EQ(alignDown(size_t{222}, size_t{100}), size_t{200});
    // Value less than alignment
    EXPECT_EQ(alignDown(size_t{6}, size_t{8}), size_t{0});
    EXPECT_EQ(alignDown(size_t{19}, size_t{30}), size_t{0});
}

/*!
 * @brief
 * Verifies that the pointer form of `alignDown` rejects a zero
 * alignment with a `std::invalid_argument` exception.
 *
 * @details
 * The pointer form is held to the same standard as the scalar
 * form. A zero alignment is invalid input for both overloads,
 * and both must refuse it with `std::invalid_argument`. The
 * address the pointer carries is not part of the decision. The
 * rejection is determined entirely by the alignment.
 *
 * The samples span the address dimension because the rejection
 * must not depend on where the pointer points. A null pointer is
 * the boundary where a fast path could slip past the guard,
 * while an aligned address and a misaligned one show that the
 * pointer's position never enters the decision.
 *
 * Each sample must throw `std::invalid_argument`. A normal return
 * from any of the calls violates the contract and fails the test.
 */
TEST(AlignDown, PointerThrowsOnZeroAlignment)
{
    constexpr size_t ALIGNMENT = 64;
    constexpr size_t BUFFER_SIZE = 128;

    alignas(ALIGNMENT) std::array<std::byte, BUFFER_SIZE> buffer{};
    std::byte* const ptr = buffer.data();
    std::byte* const nullPtr = nullptr;

    // Null pointer address
    EXPECT_THROW(alignDown(nullPtr, size_t{0}), std::invalid_argument);
    // Aligned address
    EXPECT_THROW(alignDown(ptr, size_t{0}), std::invalid_argument);
    // Misaligned address
    EXPECT_THROW(alignDown(ptr + 7, size_t{0}), std::invalid_argument);
}

/*!
 * @brief
 * Verifies that the pointer form of `alignDown` returns an
 * already aligned address unchanged.
 *
 * @details
 * The pointer form applies the identity rule to addresses. A
 * pointer that sits on a multiple of the alignment is already
 * where the operation would leave it, so the function must
 * return it exactly as it received it. A null pointer is no
 * exception. A pointer that holds no address must come back
 * holding no address, because there is nothing for the operation
 * to move.
 *
 * The samples cover both members of the already-aligned group.
 * The null samples pin the identity for a pointer with no
 * address across two alignment values, and the aligned samples
 * pin it for a boundary-exact address across the alignment span
 * the buffer can express, from the identity alignment up to the
 * buffer's own alignment.
 *
 * Each sample must return the address unchanged. Any altered
 * result violates the contract and fails the test.
 */
TEST(AlignDown, PointerReturnsSameWhenAlreadyAligned)
{
    constexpr size_t ALIGNMENT = 64;
    constexpr size_t BUFFER_SIZE = 128;

    alignas(ALIGNMENT) std::array<std::byte, BUFFER_SIZE> buffer{};
    std::byte* const ptr = buffer.data();
    std::byte* const nullPtr = nullptr;

    // Null pointer address
    EXPECT_EQ(alignDown(nullPtr, size_t{4}), nullPtr);
    EXPECT_EQ(alignDown(nullPtr, size_t{32}), nullPtr);
    // Aligned address
    EXPECT_EQ(alignDown(ptr, size_t{1}), ptr);
    EXPECT_EQ(alignDown(ptr, size_t{16}), ptr);
    EXPECT_EQ(alignDown(ptr, size_t{64}), ptr);
}

/*!
 * @brief
 * Verifies that the pointer form of `alignDown` rounds a
 * misaligned address down to the nearest multiple of the
 * alignment.
 *
 * @details
 * The pointer form of the rounding rule sends a misaligned
 * address to the multiple beneath it. An address that has passed
 * a boundary falls back to that boundary, and an address still
 * inside the first window falls to the base of the buffer, which
 * is the only multiple below it. The address must never rise,
 * and it must never fall further than the boundary it belongs
 * to.
 *
 * The samples place pointers on both sides of the boundary a
 * misaligned position can sit relative to. Each side carries a
 * case that a nearest-rounding implementation would answer
 * wrong: an address more than halfway through its window tempts
 * the implementation upward, but the contract demands the
 * multiple below. The ordinary samples sit closer to the
 * boundary below, where both readings agree, so the claim is not
 * built on adversarial points alone.
 *
 * Each sample must round the address down to the nearest
 * multiple of the alignment. Any other result violates the
 * contract and fails the test.
 */
TEST(AlignDown, PointerRoundsDownToNearestMultiple)
{
    constexpr size_t ALIGNMENT = 64;
    constexpr size_t BUFFER_SIZE = 128;

    alignas(ALIGNMENT) std::array<std::byte, BUFFER_SIZE> buffer{};
    std::byte* const ptr = buffer.data();

    // Offset greater than alignment
    EXPECT_EQ(alignDown(ptr + 50, size_t{32}), ptr + 32);
    EXPECT_EQ(alignDown(ptr + 90, size_t{64}), ptr + 64);
    // Offset less than alignment
    EXPECT_EQ(alignDown(ptr + 10, size_t{16}), ptr);
    EXPECT_EQ(alignDown(ptr + 2, size_t{64}), ptr);
}

/*!
 * @brief
 * Verifies that the pointer form of `alignDown` rounds an
 * address without ever dereferencing the pointer.
 *
 * @details
 * The pointer form works with the address the pointer holds,
 * not with whatever sits behind it. The pointed-to object is
 * never part of the operation, and the function must not reach
 * through the pointer to read or write it. This test hands the
 * function a pointer whose address cannot be touched: if the
 * implementation stays on the arithmetic, the correctly rounded
 * address comes back, and if it ever dereferences, the process
 * faults and the test fails on its own.
 *
 * A single sample is all the property warrants. Dereferencing is
 * a binary behavior, and no conditional reading of the contract
 * would make access acceptable for some alignments and not
 * others. One call with a valid alignment pins both halves of
 * the claim: no fault, and the address rounded correctly. The
 * sample is deliberately placed so that rounding downward
 * produces a non-null result, keeping the assertion meaningful
 * rather than collapsing into the null-pointer identity.
 *
 * The pointer must never be dereferenced. Any access to the
 * pointed-to memory violates the contract and fails the test.
 */
TEST(AlignDown, PointerRoundsWithoutDereference)
{
    std::byte* const illegalPtr = reinterpret_cast<std::byte*>(0x11);

    // Illegal address
    EXPECT_EQ(alignDown(illegalPtr, size_t{16}), illegalPtr - 1);
}
