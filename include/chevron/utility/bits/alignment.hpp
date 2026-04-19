
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
  * @file alignment.hpp
  *
  * @brief
  * Generic alignment utilities for sizes, offsets, and addresses.
  *
  * @author
  * Jamon T. Bailey
  *
  * @date 04-18-2026
  */

#ifndef CHEVRON_LIB_HDR_GENERAL_ALIGNMENT_UTILITIES_H_
#define CHEVRON_LIB_HDR_GENERAL_ALIGNMENT_UTILITIES_H_

#include <cstdint>
#include <cstddef>
#include <stdexcept>

namespace chevron::bits
{

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
constexpr size_t alignUp(const size_t value, const size_t alignment)
{
    if (alignment == 0)
        throw std::invalid_argument{ "alignUp: Alignment must be non-zero." };

    const size_t remainder = value % alignment;

    if (remainder == 0)
        return value;
    
    return value + (alignment - remainder);
}

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
template <typename T>
T* alignUp(const T* ptr, const size_t alignment)
{
    const uintptr_t address = static_cast<uintptr_t>(ptr);
    const uintptr_t aligned = static_cast<uintptr_t>(
        alignUp(static_cast<size_t>(address), alignment)
    );
    return reinterpret_cast<T*>(aligned);
}

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
template <typename T>
T* alignUp(const T* ptr)
{
    const uintptr_t address = static_cast<uintptr_t>(ptr);
    const uintptr_t aligned = static_cast<uintptr_t>(
        alignUp(static_cast<size_t>(address), alignof(T))
    );
    return reinterpret_cast<T*>(aligned);
}

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
constexpr size_t alignDown(const size_t value, const size_t alignment)
{
    if (alignment == 0)
        throw std::invalid_argument{ "alignDown: Alignment must be non-zero." };

    return value - (value % alignment);
}

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
template <typename T>
T* alignDown(const T* ptr, const size_t alignment)
{
    const uintptr_t address = static_cast<uintptr_t>(ptr);
    const uintptr_t aligned = static_cast<uintptr_t>(
        alignDown(static_cast<size_t>(address), alignment)
    );
    return reinterpret_cast<T*>(aligned);
}

/*!
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 */
template <typename T>
T* alignDown(const T* ptr)
{
    const uintptr_t address = static_cast<uintptr_t>(ptr);
    const uintptr_t aligned = static_cast<uintptr_t>(
        alignDown(static_cast<size_t>(address), alignof(T))
    );
    return reinterpret_cast<T*>(aligned);
}

}

#endif // CHEVRON_LIB_HDR_GENERAL_ALIGNMENT_UTILITIES_H_
