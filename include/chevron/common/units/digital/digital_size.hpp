
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
    //      <> chevron::units::DigitalSize | [PRIVATE] ATTRIBUTES
    // ===================================================================================== //
private:
    ReprType unitCount_;
};

// ===================================================================================== //
//      <> chevron::units::DigitalSize | TYPE TRAITS
// ===================================================================================== //

namespace traits {

/*!
 * @brief
 * Verify conversion between provided digital unit types is lossless.
 */
template <typename From_Units, typename To_Units>
constexpr bool is_lossless_size_conversion_v = std::ratio_divide<
    typename From_Units::ByteRatio, typename To_Units::ByteRatio>::den == 1;

/*!
 * @brief
 * Determine which of two digital units is smaller.
 */
template <typename Units_A, typename Units_B>
using finer_unit_t = std::conditional_t<
    std::ratio_less_v<typename Units_A::ByteRatio, typename Units_B::ByteRatio>, Units_A, Units_B>;

/*!
 * @brief
 * Determine which of two digital units is larger.
 */
template <typename Units_A, typename Units_B>
using coarser_unit_t = std::conditional_t<
    !std::ratio_less_v<typename Units_A::ByteRatio, typename Units_B::ByteRatio>, Units_A, Units_B>;

}

}

#endif // CHEVRON_LIB_HDR_DIGITAL_SIZE_TEMPLATE_H_
