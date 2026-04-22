
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file concepts.hpp
 *
 * @brief
 * Defines function pointer related entity concepts.
 */

#ifndef CHEVRON_LIB_HDR_FUNCTION_CONCEPTS_H_
#define CHEVRON_LIB_HDR_FUNCTION_CONCEPTS_H_

#include <concepts>
#include <type_traits>

namespace chevron::func::concepts
{

/*! @brief Concept that validates indexing into function parameters. */
template <std::size_t Index, typename... ArgsT>
concept valid_arguments_index = Index >= 0 && Index < sizeof...(ArgsT);

} // namespace chevron::func::concepts

#endif // CHEVRON_LIB_HDR_FUNCTION_CONCEPTS_H_
