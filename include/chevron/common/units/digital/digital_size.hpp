
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file digital_size.hpp
 *
 * @brief
 * Digital size quantity template.
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 04-17-2026
 */

#ifndef CHEVRON_LIB_HDR_DIGITAL_SIZE_TEMPLATE_H_
#define CHEVRON_LIB_HDR_DIGITAL_SIZE_TEMPLATE_H_

#include <cstdint>
#include <ratio>
#include <limits>
#include <algorithm>
#include <stdexcept>
#include "chevron/common/units/digital/unit_cast.hpp"
#include "chevron/common/units/digital/size_concepts.hpp"
#include "chevron/common/export.h"

namespace chevron::units
{

/*!
 * @brief
 * Binary-based digital size unit magnitude.
 */
inline constexpr CHEVRON_HEADER_API uint64_t BINARY_DIGITAL_UNIT_MAGNITUDE = 1024;

/*!
 * @brief
 * Decimal-based digital size unit magnitude.
 */
inline constexpr CHEVRON_HEADER_API uint64_t DECIMAL_DIGITAL_UNIT_MAGNITUDE = 1000;

/*!
 * @brief
 * Byte-based digital size quantity template.
 *
 * @details
 * N/a
 */
template <uint64_t UnitBytes>
class DigitalSize {
    // ===================================================================================== //
    //      <> chevron::units::DigitalSize | [PRIVATE] NESTED TYPES
    // ===================================================================================== //

    /*! @brief Highest digital size resolution (Bytes). */
    using ByteResolution = DigitalSize<1ULL>;

    // ===================================================================================== //
    //      <> chevron::units::DigitalSize | [PUBLIC] NESTED TYPES
    // ===================================================================================== //
public:
    /*! @brief Alias of underlying byte representation. */
    using ReprType = uint64_t;
    /*! @brief Ratio detailing total bytes in this unit. */
    using ByteRatio = std::ratio<UnitBytes, 1>;

    // ===================================================================================== //
    //      <> chevron::units::DigitalSize | CONSTRUCTORS / DESTRUCTOR
    // ===================================================================================== //

    /*! @brief Default construct with a digital size of 0 bytes. */
    constexpr DigitalSize() noexcept = default;

    /*!
     * @brief
     * Construct digital size quantity with specified unit count.
     */
    constexpr explicit DigitalSize(ReprType units) noexcept
        : unitCount_{units}
    {
        //
    }

    /*!
     * @brief
     * Construct digital size quantity with different size units
     * (lossy).
     */
    template <uint64_t IncomingUnitBytes>
    constexpr explicit DigitalSize(const DigitalSize<IncomingUnitBytes>& other)
        : unitCount_{DigitalSizeCast<DigitalSize<UnitBytes>>::cast(other).count()}
    {
        //
    }

    /*!
     * @brief
     * Construct digital size quantity with different size units
     * (lossless).
     */
    template <uint64_t IncomingUnitBytes>
    constexpr DigitalSize(const DigitalSize<IncomingUnitBytes>& other)
        requires concepts::lossless_digital_size_conversion<DigitalSize<IncomingUnitBytes>, DigitalSize<UnitBytes>>
        : unitCount_{DigitalSizeCast<DigitalSize<UnitBytes>>::cast(other).count()}
    {
        //
    }

    ~DigitalSize() noexcept = default;

    // ===================================================================================== //
    //      <> chevron::units::DigitalSize | [PUBLIC] MEMBER METHODS
    // ===================================================================================== //

    /*!
     * @brief
     * Returns raw count of digital units.
     * 
     * @return
     * Raw count of digital units
     */
    constexpr ReprType count() const noexcept
    {
        return unitCount_;
    }

    /*!
     * @brief
     * Returns total number of bytes in this size.
     * 
     * @return
     * This size in bytes
     */
    constexpr ReprType bytes() const noexcept
    {
        return unitCount_ * ByteRatio::num / ByteRatio::den;
    }

    /*!
     * @brief
     * Returns total number of bytes in this size as size_t.
     * 
     * @return
     * This size in bytes
     */
    constexpr size_t size_t_bytes() const noexcept(sizeof(size_t) >= sizeof(ReprType))
    {
        const ReprType byteCount = bytes();

        if constexpr (sizeof(size_t) < sizeof(ReprType)) {
            if (byteCount > std::numeric_limits<size_t>::max()) {
                throw std::overflow_error(
                    "DigitalSize::size_t_bytes(): Byte count exceeds size_t capacity"
                );
            }
        }

        return static_cast<size_t>(byteCount);
    }

    /*!
     * @brief
     * Calculates absolute difference between this and
     * another digital size.
     * 
     * @details
     * The difference operation conducted in this method
     * is direction independent. It returns the positive
     * difference between the two regardless of which is
     * larger. This is meaningful when the question is
     * "how far apart are two digital sizes?".
     * 
     * @return
     * Absolute difference between DigitalSize values
     */
    template <uint64_t IncomingUnitBytes>
    constexpr auto absoluteDifference(const DigitalSize<IncomingUnitBytes>& other) const noexcept
    {
        using Self = DigitalSize<UnitBytes>;
        using Other = DigitalSize<IncomingUnitBytes>;
        using FinerUnit = traits::finer_size_t<Self, Other>;

        const ReprType selfUnits = DigitalSizeCast<FinerUnit>::cast(*this).count();
        const ReprType otherUnits = DigitalSizeCast<FinerUnit>::cast(other).count();
        return FinerUnit{
            std::max(selfUnits, otherUnits) - std::min(selfUnits, otherUnits)
        };
    }

    // ===================================================================================== //
    //      <> chevron::units::DigitalSize | OPERATORS
    // ===================================================================================== //

    /*!
     * @brief
     * Tests whether this and another digital size represent
     * the same byte count.
     * 
     * @details
     * The comparison is performed in bytes, so values from
     * different units compare as equal when they represent
     * the same size (i.e. `1_MiB == 1024_KiB` is a true
     * statement).
     * 
     * @return
     * True if equivalent sizes
     */
    template <uint64_t IncomingUnitBytes>
    constexpr bool operator==(const DigitalSize<IncomingUnitBytes>& other) const noexcept
    {
        return this->bytes() == other.bytes();
    }

    /*!
     * @brief
     * Tests whether this digital size is strictly less than
     * another.
     * 
     * @return
     * True if left-hand side is smaller
     */
    template <uint64_t IncomingUnitBytes>
    constexpr bool operator<(const DigitalSize<IncomingUnitBytes>& other) const noexcept
    {
        return this->bytes() < other.bytes();
    }

    /*!
     * @brief
     * Tests whether this digital size is strictly greater
     * than another.
     *
     * @return
     * True if left-hand side is greater
     */
    template <uint64_t IncomingUnitBytes>
    constexpr bool operator>(const DigitalSize<IncomingUnitBytes>& other) const noexcept
    {
        return this->bytes() > other.bytes();
    }

    /*!
     * @brief
     * Adds another digital size of any unit to this one.
     * 
     * @details
     * Same-system arithmetic returns the finer unit of
     * the two types. Cross-system arithmetic returns
     * `Bytes` since this is the only unit the binary
     * and decimal size systems can represent exact.
     * Addition conducted through this operator is
     * always lossless.
     * 
     * @return
     * Sum of two digital sizes in the greatest common unit
     */
    template <uint64_t IncomingUnitBytes>
    constexpr auto operator+(const DigitalSize<IncomingUnitBytes>& other) const noexcept
    {
        using Self = DigitalSize<UnitBytes>;
        using Other = DigitalSize<IncomingUnitBytes>;

        if constexpr (concepts::matching_digital_size_systems<Self, Other>) {
            using FinerUnit = traits::finer_size_t<Self, Other>;

            const FinerUnit lhs = DigitalSizeCast<FinerUnit>::cast(*this);
            const FinerUnit rhs = DigitalSizeCast<FinerUnit>::cast(other);
            return FinerUnit{ lhs.count() + rhs.count() };
        }
        else {
            return ByteResolution{ this->bytes() + other.bytes() };
        }
    }

    /*!
     * @brief
     * Subtracts another digital size of any unit from
     * this one.
     * 
     * @details
     * Same-system arithmetic returns the finer unit of
     * the two types. Cross-system arithmetic returns
     * `Bytes` since this is the only unit the binary
     * and decimal size systems can represent exact.
     * Subtraction conducted through this operator is
     * always lossless.
     * 
     * @return
     * Difference of two digital sizes in the greatest
     * common unit
     */
    template <uint64_t IncomingUnitBytes>
    constexpr auto operator-(const DigitalSize<IncomingUnitBytes>& other) const noexcept
    {
        using Self = DigitalSize<UnitBytes>;
        using Other = DigitalSize<IncomingUnitBytes>;

        if constexpr (concepts::matching_digital_size_systems<Self, Other>) {
            using FinerUnit = traits::finer_size_t<Self, Other>;

            const FinerUnit lhs = DigitalSizeCast<FinerUnit>::cast(*this);
            const FinerUnit rhs = DigitalSizeCast<FinerUnit>::cast(other);
            return FinerUnit{ lhs.count() - rhs.count() };
        }
        else {
            return ByteResolution{ this->bytes() - other.bytes() };
        }
    }

    /*!
     * @brief
     * Calculates remainder of dividing this digital size
     * by another.
     * 
     * @details
     * Same-system arithmetic returns the finer unit of
     * the two types. Cross-system arithmetic returns
     * `Bytes` since this is the only unit the binary
     * and decimal size systems can represent exact.
     * Modulo conducted through this operator is always
     * lossless.
     *
     * @return
     * Remainder of division between digital sizes
     */
    template <uint64_t IncomingUnitBytes>
    constexpr auto operator%(const DigitalSize<IncomingUnitBytes>& other) const
    {
        using Self = DigitalSize<UnitBytes>;
        using Other = DigitalSize<IncomingUnitBytes>;

        if constexpr (concepts::matching_digital_size_systems<Self, Other>) {
            using FinerUnit = traits::finer_size_t<Self, Other>;

            const FinerUnit lhs = DigitalSizeCast<FinerUnit>::cast(*this);
            const FinerUnit rhs = DigitalSizeCast<FinerUnit>::cast(other);

            if (rhs.count() == 0)
                throw std::invalid_argument{"DigitalSize::operator%(): Division by zero."};

            return FinerUnit{ lhs.count() % rhs.count() };
        }
        else {
            if (other.bytes() == 0)
                throw std::invalid_argument{"DigitalSize::operator%(): Division by zero."};

            return ByteResolution{ this->bytes() % other.bytes() };
        }
    }

    /*!
     * @brief
     * Multiplies this digital size by a scalar.
     * 
     * @note
     * The unit type is always preserved during this
     * operation; only the unit count is scaled.
     * 
     * @return
     * Scaled digital size in same units
     */
    constexpr DigitalSize<UnitBytes> operator*(const ReprType scalar) const noexcept
    {
        return DigitalSize<UnitBytes>{this->unitCount_ * scalar};
    }

    /*!
     * @brief
     * Divides this digital size by a scalar.
     * 
     * @note
     * The unit type is always preserved during this
     * operation; only the unit count is scaled.
     * 
     * @return
     * Scaled digital size in same units
     */
    constexpr DigitalSize<UnitBytes> operator/(const ReprType scalar) const
    {
        if (scalar == 0)
            throw std::invalid_argument{"DigitalSize::operator/(): Division by zero."};

        return DigitalSize<UnitBytes>{this->unitCount_ / scalar};
    }

    /*!
     * @brief
     * Divides this digital size by another.
     * 
     * @details
     * Both operands are converted to bytes before division.
     * The result is a `double` to preserve precision in the
     * resulting quotient.
     * 
     * @return
     * Quotient of two digital sizes as a double
     */
    template <uint64_t IncomingUnitBytes>
    constexpr double operator/(const DigitalSize<IncomingUnitBytes>& other) const
    {
        if (other.count() == 0)
            throw std::invalid_argument{"DigitalSize::operator/(): Division by zero."};

        return static_cast<double>(this->bytes()) / static_cast<double>(other.bytes());
    }

    /*!
     * @brief
     * Assigns another digital size to this.
     *
     * @details
     * This operator is only applicable to lossless
     * assignment operations.
     */
    template <uint64_t IncomingUnitBytes>
    DigitalSize<UnitBytes>& operator=(const DigitalSize<IncomingUnitBytes>& other) noexcept
        requires concepts::lossless_digital_size_conversion<DigitalSize<IncomingUnitBytes>, DigitalSize<UnitBytes>>
    {
        using Self = DigitalSize<UnitBytes>;

        this->unitCount_ = DigitalSizeCast<Self>::cast(other).count();
        return *this;
    }

    // ===================================================================================== //
    //      <> chevron::units::DigitalSize | COMPOUND OPERATORS
    // ===================================================================================== //

    /*!
     * @brief
     * Tests whether this digital size is less than or equal
     * to another.
     *
     * @return
     * True if left-hand side is smaller or equal
     */
    template <uint64_t IncomingUnitBytes>
    constexpr bool operator<=(const DigitalSize<IncomingUnitBytes>& other) const noexcept
    {
        return this->bytes() <= other.bytes();
    }

    /*!
     * @brief
     * Tests whether this digital size is greater than or
     * equal to another.
     *
     * @return
     * True if left-hand side is greater or equal
     */
    template <uint64_t IncomingUnitBytes>
    constexpr bool operator>=(const DigitalSize<IncomingUnitBytes>& other) const noexcept
    {
        return this->bytes() >= other.bytes();
    }

    /*!
     * @brief
     * In-place addition.
     * 
     * @details
     * The right-hand side is constrained to the resolution
     * of the left-hand side units during an in-place
     * addition operation.
     * 
     * @note
     * The unit type is always preserved during in-place
     * operations.
     */
    template <uint64_t IncomingUnitBytes>
    DigitalSize<UnitBytes>& operator+=(const DigitalSize<IncomingUnitBytes>& other) noexcept
    {
        using Self = DigitalSize<UnitBytes>;

        this->unitCount_ += DigitalSizeCast<Self>::cast(other).count();
        return *this;
    }

    /*!
     * @brief
     * In-place subtraction.
     * 
     * @details
     * The right-hand side is constrained to the resolution
     * of the left-hand side units during an in-place
     * subtraction operation.
     * 
     * @note
     * The unit type is always preserved during in-place
     * operations.
     */
    template <uint64_t IncomingUnitBytes>
    DigitalSize<UnitBytes>& operator-=(const DigitalSize<IncomingUnitBytes>& other) noexcept
    {
        using Self = DigitalSize<UnitBytes>;

        this->unitCount_ -= DigitalSizeCast<Self>::cast(other).count();
        return *this;
    }

    /*!
     * @brief
     * In-place modulus division.
     *
     * @details
     * This operator is only available when the right-hand
     * side is losslessly convertible to this types units.
     * Combinations that would truncate the divisor are
     * rejected from using this signature. In those cases
     * the caller must use `operator%()` and store that
     * result in another variable. In-place operators are
     * intentionally designed to preserve this types units.
     * 
     * @note
     * The unit type is always preserved during in-place
     * operations.
     */
    template <uint64_t IncomingUnitBytes>
    DigitalSize<UnitBytes>& operator%=(const DigitalSize<IncomingUnitBytes>& other)
        requires concepts::lossless_digital_size_conversion<DigitalSize<IncomingUnitBytes>, DigitalSize<UnitBytes>>
    {
        using Self = DigitalSize<UnitBytes>;

        const ReprType otherUnitCount = DigitalSizeCast<Self>::cast(other).count();

        if (otherUnitCount == 0)
            throw std::invalid_argument{"DigitalSize::operator%=(): Division by zero."};

        this->unitCount_ %= otherUnitCount;
        return *this;
    }

    /*!
     * @brief
     * In-place scalar multiplication.
     * 
     * @note
     * The unit type is always preserved during in-place
     * operations.
     */
    DigitalSize<UnitBytes>& operator*=(const ReprType scalar) noexcept
    {
        this->unitCount_ *= scalar;
        return *this;
    }

    /*!
     * @brief
     * In-place scalar division.
     * 
     * @note
     * The unit type is always preserved during in-place
     * operations.
     */
    DigitalSize<UnitBytes>& operator/=(const ReprType scalar)
    {
        if (scalar == 0)
            throw std::invalid_argument{"DigitalSize::operator/=(): Division by zero."};

        this->unitCount_ /= scalar;
        return *this;
    }

    // ===================================================================================== //
    //      <> chevron::units::DigitalSize | [PRIVATE] ATTRIBUTES
    // ===================================================================================== //
private:
    ReprType unitCount_;
};

namespace traits {

/* ------------------------------------------------------------------------------------- */
//      > is_digital_size | TYPE TRAIT
/* ------------------------------------------------------------------------------------- */

/*!
 * @brief
 * Base specialization of `DigitalSize` type validation.
 */
template <typename T>
struct is_digital_size : std::false_type {};

/*!
 * @brief
 * Specialization for any `DigitalSize` instantiation.
 */
template <uint64_t UInt>
struct is_digital_size<DigitalSize<UInt>> : std::true_type {};

/*!
 * @brief
 * Convenience alias for `DigitalSize` type validation.
 */
template <typename T>
constexpr bool is_digital_size_v = is_digital_size<T>::value;

}

}

#endif // CHEVRON_LIB_HDR_DIGITAL_SIZE_TEMPLATE_H_
