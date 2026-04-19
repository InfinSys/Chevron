
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

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
template <uint64_t Base, uint64_t Expon>
constexpr uint64_t exponent = []() {
    uint64_t result = 1;

    for (uint64_t i = 0; i < Expon; ++i)
        result *= Base;
    
    return result;
}();

}

#endif // CHEVRON_LIB_HDR_ALGEBRAIC_COMPUTATION_H_
