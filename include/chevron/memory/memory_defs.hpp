
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
  * @file memory_defs.hpp
  *
  * @brief
  * TODO: INCOMPLETE DOCUMENTATION!!!
  *
  * @author
  * Jamon T. Bailey
  *
  * @date 04-30-2026
  */

#ifndef CHEVRON_LIB_MEMORY_DEFINITIONS_H_
#define CHEVRON_LIB_MEMORY_DEFINITIONS_H_

#include <cstdint>
#include <atomic>
#include "chevron/thread/utility/tagged_ptr.hpp"

namespace chevron::memory
{

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 * 
 * @details
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
struct FreeRegionNode {
	void* next;   ///< Pointer to next free block
};

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 * 
 * @details
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
using AtomicFreeListHead = std::atomic<utility::TaggedPointer<void>>;

}

#endif // CHEVRON_LIB_MEMORY_DEFINITIONS_H_
