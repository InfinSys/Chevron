
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file mem_pool_config.cpp
 *
 * @brief
 * // TODO: INCOMPLETE DOC STRING!!!
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 04-14-2026
 */

#include <stdexcept>
#include "chevron/process/memory/mem_pool_config.hpp"

// ===================================================================================== //
//      <> chevron::process::MemoryPoolConfig | [PUBLIC] MEMBER METHODS
// ===================================================================================== //

bool chevron::process::MemoryPoolConfig::isValid() const noexcept
{
    return validate(nullptr);
}

void chevron::process::MemoryPoolConfig::isValidOrThrow() const
{
    const char* reason = nullptr;

    if (!validate(&reason))
        throw std::invalid_argument(reason);
}

// ===================================================================================== //
//      <> chevron::process::MemoryPoolConfig | [PRIVATE] MEMBER METHODS
// ===================================================================================== //

bool chevron::process::MemoryPoolConfig::validate(const char** reason) const noexcept
{
    // TODO: INCOMPLETE IMPLEMENTATION!!!
    return false;
}
