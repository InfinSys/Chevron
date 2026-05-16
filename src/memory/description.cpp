
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file description.cpp
 *
 * @brief
 * TODO: INCOMPLETE DOCUMENTATION!!!
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 04-28-2026
 */

#include "chevron/memory/description.hpp"

// ===================================================================================== //
//      <> chevron::memory::MemoryDescription | CONSTRUCTORS / DESTRUCTOR
// ===================================================================================== //

chevron::memory::MemoryDescription::MemoryDescription(void* base, size_t alignment, units::Bytes size) noexcept
	: baseAddress_{base}, alignment_{alignment}, capacity_{size.size_t_bytes()}
{
	//
}

// ===================================================================================== //
//      <> chevron::memory::MemoryDescription | [PUBLIC] MEMBER METHODS
// ===================================================================================== //

void* chevron::memory::MemoryDescription::base() const noexcept
{
	return baseAddress_;
}

size_t chevron::memory::MemoryDescription::alignment() const noexcept
{
	return alignment_;
}

chevron::units::Bytes chevron::memory::MemoryDescription::size() const noexcept
{
	return units::Bytes{capacity_};
}

size_t chevron::memory::MemoryDescription::length() const noexcept
{
	return capacity_;
}

std::byte* chevron::memory::MemoryDescription::begin() const noexcept
{
	return static_cast<std::byte*>(baseAddress_);
}

std::byte* chevron::memory::MemoryDescription::end() const noexcept
{
	return static_cast<std::byte*>(baseAddress_) + capacity_;
}

bool chevron::memory::MemoryDescription::containsAddress(const void* ptr) const noexcept
{
	if (!ptr) return false;

	const std::byte* givenAddress = static_cast<const std::byte*>(ptr);
	return givenAddress >= this->begin() && givenAddress < this->end();
}

bool chevron::memory::MemoryDescription::isAlignedTo(const size_t alignment) const noexcept
{
	return (reinterpret_cast<uintptr_t>(baseAddress_) % alignment) == 0;
}

void chevron::memory::MemoryDescription::invalidate() noexcept
{
	baseAddress_ = nullptr;
	alignment_ = 0;
	capacity_ = 0;
}
