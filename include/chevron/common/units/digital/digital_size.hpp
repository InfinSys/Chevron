
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

namespace chevron::units
{

/*!
 * @brief
 * Binary-based digital size unit magnitude.
 */
inline constexpr uint64_t BINARY_DIGITAL_UNIT_MAGNITUDE = 1024;

/*!
 * @brief
 * Decimal-based digital size unit magnitude.
 */
inline constexpr uint64_t DECIMAL_DIGITAL_UNIT_MAGNITUDE = 1000;

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
     * Construct digital size quantity with different size units (lossy).
     */
    template <uint64_t IncomingUnitBytes>
    constexpr explicit DigitalSize(const DigitalSize<IncomingUnitBytes>& other)
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
     * TODO: INCOMPLETE DOCUMENTATION!!!
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
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    template <uint64_t IncomingUnitBytes>
    constexpr bool operator==(const DigitalSize<IncomingUnitBytes>& other) const noexcept
    {
        return this->bytes() == other.bytes();
    }

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    template <uint64_t IncomingUnitBytes>
    constexpr bool operator<(const DigitalSize<IncomingUnitBytes>& other) const noexcept
    {
        return this->bytes() < other.bytes();
    }

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    template <uint64_t IncomingUnitBytes>
    constexpr bool operator>(const DigitalSize<IncomingUnitBytes>& other) const noexcept
    {
        return this->bytes() > other.bytes();
    }

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
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
     * TODO: INCOMPLETE DOCUMENTATION!!!
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
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    constexpr DigitalSize<UnitBytes> operator*(const ReprType scalar) const noexcept
    {
        return DigitalSize<UnitBytes>{this->unitCount_ * scalar};
    }

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    constexpr DigitalSize<UnitBytes> operator/(const ReprType scalar) const
    {
        if (scalar == 0)
            throw std::invalid_argument{"DigitalSize::operator/(): Division by zero."};

        return DigitalSize<UnitBytes>{this->unitCount_ / scalar};
    }

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
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
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    template <uint64_t IncomingUnitBytes>
    constexpr ReprType operator%(const DigitalSize<IncomingUnitBytes>& other) const
    {
        using Self = DigitalSize<UnitBytes>;

        const Self castedOther = DigitalSizeCast<Self>::cast(other);

        if (castedOther.count() == 0)
            throw std::invalid_argument{"DigitalSize::operator%(): Division by zero."};

        return static_cast<ReprType>(this->unitCount_ % castedOther.count());
    }

    // ===================================================================================== //
    //      <> chevron::units::DigitalSize | COMPOUND OPERATORS
    // ===================================================================================== //

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    template <uint64_t IncomingUnitBytes>
    constexpr bool operator<=(const DigitalSize<IncomingUnitBytes>& other) const noexcept
    {
        return this->bytes() <= other.bytes();
    }

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    template <uint64_t IncomingUnitBytes>
    constexpr bool operator>=(const DigitalSize<IncomingUnitBytes>& other) const noexcept
    {
        return this->bytes() >= other.bytes();
    }

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
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
     * TODO: INCOMPLETE DOCUMENTATION!!!
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
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    DigitalSize<UnitBytes>& operator*=(const ReprType scalar) noexcept
    {
        this->unitCount_ *= scalar;
        return *this;
    }

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    DigitalSize<UnitBytes>& operator/=(const ReprType scalar)
    {
        if (scalar == 0) {
            throw std::invalid_argument{ "DigitalSize::operator/=(): Division by zero." };
        }

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
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
template <typename T>
struct is_digital_size : std::false_type {};

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
template <uint64_t UInt>
struct is_digital_size<DigitalSize<UInt>> : std::true_type {};

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
template <typename T>
constexpr bool is_digital_size_v = is_digital_size<T>::value;

}

}

#endif // CHEVRON_LIB_HDR_DIGITAL_SIZE_TEMPLATE_H_
