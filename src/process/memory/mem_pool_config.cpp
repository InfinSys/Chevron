
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file mem_pool_config.cpp
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 04-14-2026
 */

#include <stdexcept>
#include "chevron/process/memory/mem_pool_config.hpp"
#include "chevron/utility/bits/powers.hpp"

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
    if (block_size.bytes() == 0) {
        if (reason) *reason = "MemoryPoolConfig: Block size must be greater than zero.";
        return false;
    }

    if (block_alignment == 0) {
        if (reason) *reason = "MemoryPoolConfig: Block alignment must be greater than zero.";
        return false;
    }

    if (!bits::isPowerOfTwo(block_alignment)) {
        if (reason) *reason = "MemoryPoolConfig: Block alignment must be a power of 2.";
        return false;
    }

    if (chunk_size < block_size) {
        if (reason) *reason = "MemoryPoolConfig: Block size cannot exceed chunk size.";
        return false;
    }

    if (budget_ceiling < chunk_size) {
        if (reason) *reason = "MemoryPoolConfig: Chunk size cannot exceed memory budget.";
        return false;
    }

    if (initial_thread_blocks == 0) {
        if (reason) *reason =
            "MemoryPoolConfig: Initial thread block count must be greater than zero.";
        return false;
    }

    if (max_thread_blocks < initial_thread_blocks) {
        if (reason) *reason =
            "MemoryPoolConfig: Initial thread block count cannot exceed maximum thread block count.";
        return false;
    }

    if (max_thread_batch_size < initial_thread_blocks) {
        if (reason) *reason =
            "MemoryPoolConfig: Initial thread block count cannot exceed maximum block batch size.";
        return false;
    }

    if (max_thread_batch_size > max_thread_blocks) {
        if (reason) *reason =
            "MemoryPoolConfig: Maximum thread block batch size cannot exceed maximum block count.";
        return false;
    }

    // No issues found
    return true;
}
