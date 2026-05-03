
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
  * @file tagged_ptr.hpp
  *
  * @brief
  * TODO: INCOMPLETE DOCUMENTATION!!!
  *
  * @author
  * Jamon T. Bailey
  *
  * @date 04-30-2026
  */

#ifndef CHEVRON_LIB_HDR_TAGGED_POINTER_H_
#define CHEVRON_LIB_HDR_TAGGED_POINTER_H_

#include <cstdint>

namespace chevron::utility
{

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 * 
 * @details
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
template <typename T>
struct alignas(16) TaggedPointer {
	T* ptr;           ///< Pointer to `T`
	uint64_t swaps;   ///< Monotonically increasing operation counter
};

}

#endif // CHEVRON_LIB_HDR_TAGGED_POINTER_H_
