
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file type_traits.hpp
 *
 * @brief
 * Defines function related entity type traits.
 */

#ifndef CHEVRON_LIB_H_FUNCTION_TYPE_TRAITS_H_
#define CHEVRON_LIB_H_FUNCTION_TYPE_TRAITS_H_

#include <concepts>
#include <type_traits>
#include <tuple>

namespace chevron::model::traits
{

/*! @brief Callable entity signature extraction utility. */
template <typename T>
struct callable_signature;

/*!
 * @brief
 * Lambda and functor signature extraction utility.
 */
template <typename T>
struct callable_signature : callable_signature<decltype(&T::operator())> { };

/*!
 * @brief
 * Non-const member method signature extraction specialization.
 */
template <typename ClassType, typename ReturnT, typename... ArgsT>
struct callable_signature<ReturnT (ClassType::*)(ArgsT...)> {
    using ReturnType = ReturnT;
    using ArgsTuple  = std::tuple<ArgsT...>;
    using FuncType   = ReturnT (ClassType::*)(ArgsT...);
    using Class      = ClassType;
};

/*!
 * @brief
 * Const member method signature extraction specialization.
 */
template <typename ClassType, typename ReturnT, typename... ArgsT>
struct callable_signature<ReturnT (ClassType::*)(ArgsT...) const> {
    using ReturnType = ReturnT;
    using ArgsTuple  = std::tuple<ArgsT...>;
    using FuncType   = ReturnT (ClassType::*)(ArgsT...) const;
    using Class      = ClassType;
};

/*!
 * @brief
 * Free method signature extraction specialization.
 */
template <typename ReturnT, typename... ArgsT>
struct callable_signature<ReturnT (*)(ArgsT...)> {
    using ReturnType = ReturnT;
    using ArgsTuple  = std::tuple<ArgsT...>;
    using FuncType   = ReturnT (*)(ArgsT...);
};

} // namespace chevron::model::traits

#endif // CHEVRON_LIB_H_FUNCTION_TYPE_TRAITS_H_
