
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
#include <stdexcept>
#include "chevron/common/units/digital/type_traits.hpp"

namespace chevron::units
{

// ===================================================================================== //
//      <> chevron::units::DigitalSize | TYPE TRAITS
// ===================================================================================== //

namespace traits {

/*!
 * @brief
 * Verify conversion between provided digital size units is lossless.
 */
template <typename From_Units, typename To_Units>
constexpr bool is_lossless_size_conversion_v = std::ratio_divide<
    typename From_Units::ByteRatio, typename To_Units::ByteRatio>::den == 1;

/*!
 * @brief
 * Determine which of two digital size units is smaller.
 */
template <typename Units_A, typename Units_B>
using finer_size_t = std::conditional_t<
    std::ratio_less_v<typename Units_A::ByteRatio, typename Units_B::ByteRatio>, Units_A, Units_B>;

/*!
 * @brief
 * Determine which of two digital size units is larger.
 */
template <typename Units_A, typename Units_B>
using coarser_size_t = std::conditional_t<
    !std::ratio_less_v<typename Units_A::ByteRatio, typename Units_B::ByteRatio>, Units_A, Units_B>;

}

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
inline constexpr uint64_t IEC_DIGITAL_UNIT_MAGNITUDE = 1024;

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
inline constexpr uint64_t STD_DIGITAL_UNIT_MAGNITUDE = 1000;

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
                    "DigitalSize::to_size_t: Byte count exceeds size_t capacity"
                );
            }
        }

        return static_cast<size_t>(byteCount);
    }

    // ===================================================================================== //
    //      <> chevron::units::DigitalSize | OPERATORS
    // ===================================================================================== //

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    template <uint64_t IncomingUnitBytes>
    constexpr auto operator+(const DigitalSize<IncomingUnitBytes>& other) const noexcept
    {
        using FinerUnit = traits::finer_size_t<DigitalSize<UnitBytes>, DigitalSize<IncomingUnitBytes>>;

        const ReprType totalBytes = bytes() + other.bytes();
        return FinerUnit{static_cast<ReprType>(totalBytes / FinerUnit::ByteRatio::num)};
    }

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    template <uint64_t IncomingUnitBytes>
    constexpr auto operator-(const DigitalSize<IncomingUnitBytes>& other) const noexcept
    {
        using FinerUnit = traits::finer_size_t<DigitalSize<UnitBytes>, DigitalSize<IncomingUnitBytes>>;

        const ReprType diffBytes = bytes() - other.bytes();
        return FinerUnit{static_cast<ReprType>(diffBytes / FinerUnit::ByteRatio::num)};
    }

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    constexpr DigitalSize<UnitBytes> operator*(const ReprType scalar) const noexcept
    {
        return DigitalSize<UnitBytes>{unitCount_ * scalar};
    }

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    constexpr DigitalSize<UnitBytes> operator/(const ReprType scalar) const
    {
        if (scalar == 0) {
            throw std::invalid_argument{"DigitalSize::operator/(): Division by zero."};
        }

        return DigitalSize<UnitBytes>{unitCount_ / scalar};
    }

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    template <uint64_t IncomingUnitBytes>
    constexpr double operator/(const DigitalSize<IncomingUnitBytes>& other) const
    {
        if (other.count() == 0) {
            throw std::invalid_argument{ "DigitalSize::operator/(): Division by zero." };
        }

        return static_cast<double>(bytes()) / static_cast<double>(other.bytes());
    }

    // ===================================================================================== //
    //      <> chevron::units::DigitalSize | COMPOUND OPERATORS
    // ===================================================================================== //

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    template <uint64_t IncomingUnitBytes>
    DigitalSize<UnitBytes>& operator+=(const DigitalSize<IncomingUnitBytes>& other) noexcept
    {
        const ReprType totalBytes = bytes() + other.bytes();
        unitCount_ = static_cast<ReprType>(totalBytes / ByteRatio::num);
        return *this;
    }

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    template <uint64_t IncomingUnitBytes>
    DigitalSize<UnitBytes>& operator-=(const DigitalSize<IncomingUnitBytes>& other) noexcept
    {
        const ReprType diffBytes = bytes() - other.bytes();
        unitCount_ = static_cast<ReprType>(diffBytes / ByteRatio::num);
        return *this;
    }

    /*!
     * @brief
     * TODO: INCOMPLETE DOCUMENTATION!!!
     */
    DigitalSize<UnitBytes>& operator*=(const ReprType scalar) noexcept
    {
        unitCount_ *= scalar;
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

        unitCount_ /= scalar;
        return *this;
    }

    // ===================================================================================== //
    //      <> chevron::units::DigitalSize | [PRIVATE] ATTRIBUTES
    // ===================================================================================== //
private:
    ReprType unitCount_;
};

}

#endif // CHEVRON_LIB_HDR_DIGITAL_SIZE_TEMPLATE_H_
