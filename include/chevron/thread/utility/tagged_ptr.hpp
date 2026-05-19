
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
  * @file tagged_ptr.hpp
  *
  * @brief
  * ABA-safe tagged pointer for lock-free CAS
  * operations.
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
 * Pointer paired with a modification counter for
 * ABA-safe CAS.
 * 
 * @details
 * Combines a typed pointer with a monotonically
 * increasing counter to detect intervening
 * modifications during compare-and-swap operations.
 * Aligned to 16 bytes to satisfy hardware requirements
 * for double-width atomic instructions.
 */
template <typename T>
struct alignas(16) TaggedPointer {
	T* ptr;           ///< Pointer to instance of `T`
	uint64_t swaps;   ///< Monotonically increasing modification counter
};

}

#endif // CHEVRON_LIB_HDR_TAGGED_POINTER_H_
