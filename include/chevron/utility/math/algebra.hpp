
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
  * @file algebra.hpp
  *
  * @brief
  * Algebraic computation utilities.
  *
  * @author
  * Jamon T. Bailey
  *
  * @date 04-18-2026
  */

#ifndef CHEVRON_LIB_HDR_ALGEBRAIC_COMPUTATION_H_
#define CHEVRON_LIB_HDR_ALGEBRAIC_COMPUTATION_H_

#include <cstdint>

namespace chevron::math
{

/* ------------------------------------------------------------------------------------- */
//      > Exponents
/* ------------------------------------------------------------------------------------- */

/*!
 * @brief
 * Computes integral exponentation on provided base and
 * exponent.
 * 
 * @return
 * = (base^expon)
 */
constexpr uint64_t exponent(const uint64_t base, const uint64_t expon)
{
    uint64_t result = 1;

    for (uint64_t i = 0; i < expon; ++i)
        result *= base;

    return result;
}

/*!
 * @brief
 * Computes integral exponentation.
 *
 * @return
 * = (Base^Expon)
 */
template <uint64_t Base, uint64_t Expon>
constexpr uint64_t exponent_v = exponent(Base, Expon);

}

#endif // CHEVRON_LIB_HDR_ALGEBRAIC_COMPUTATION_H_
