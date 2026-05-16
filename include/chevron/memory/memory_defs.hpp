
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
  * @file memory_defs.hpp
  *
  * @brief
  * Core type definitions for Chevron's memory
  * infrastructure.
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

namespace chevron::memory
{

/*!
 * @brief
 * Embedded free list node for overlaying on free memory
 * blocks.
 * 
 * @details
 * When a block of memory is free, its first bytes are
 * reinterpreted as this structure, threading it into a
 * free list.
 */
struct FreeRegionNode {
	void* next;   ///< Pointer to next free block
};

}

#endif // CHEVRON_LIB_MEMORY_DEFINITIONS_H_
