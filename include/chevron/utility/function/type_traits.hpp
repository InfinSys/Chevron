
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file type_traits.hpp
 *
 * @brief
 * Defines function related entity type traits.
 */

#ifndef CHEVRON_LIB_HDR_FUNCTION_TYPE_TRAITS_H_
#define CHEVRON_LIB_HDR_FUNCTION_TYPE_TRAITS_H_

#include <concepts>
#include <type_traits>
#include <tuple>

namespace chevron::func::traits
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
 * Non-const member method signature specialization.
 */
template <typename ClassType, typename ReturnT, typename... ArgsT>
struct callable_signature<ReturnT (ClassType::*)(ArgsT...)> {
    using ReturnType   = ReturnT;
    using ArgsTuple    = std::tuple<ArgsT...>;
    using FuncType     = ReturnT (ClassType::*)(ArgsT...);
    using Class        = ClassType;
    using IsNoexcept_t = std::false_type;
};

/*!
 * @brief
 * Non-const noexcept member method signature specialization.
 */
template <typename ClassType, typename ReturnT, typename... ArgsT>
struct callable_signature<ReturnT (ClassType::*)(ArgsT...) noexcept> {
    using ReturnType   = ReturnT;
    using ArgsTuple    = std::tuple<ArgsT...>;
    using FuncType     = ReturnT (ClassType::*)(ArgsT...) noexcept;
    using Class        = ClassType;
    using IsNoexcept_t = std::true_type;
};

/*!
 * @brief
 * Const member method signature specialization.
 */
template <typename ClassType, typename ReturnT, typename... ArgsT>
struct callable_signature<ReturnT (ClassType::*)(ArgsT...) const> {
    using ReturnType   = ReturnT;
    using ArgsTuple    = std::tuple<ArgsT...>;
    using FuncType     = ReturnT (ClassType::*)(ArgsT...) const;
    using Class        = ClassType;
    using IsNoexcept_t = std::false_type;

    static constexpr bool IsNoexcept_v = IsNoexcept_t::value;
};

/*!
 * @brief
 * Const noexcept member method signature specialization.
 */
template <typename ClassType, typename ReturnT, typename... ArgsT>
struct callable_signature<ReturnT (ClassType::*)(ArgsT...) const noexcept> {
    using ReturnType   = ReturnT;
    using ArgsTuple    = std::tuple<ArgsT...>;
    using FuncType     = ReturnT (ClassType::*)(ArgsT...) const noexcept;
    using Class        = ClassType;
    using IsNoexcept_t = std::true_type;

    static constexpr bool IsNoexcept_v = IsNoexcept_t::value;
};

/*!
 * @brief
 * Free function signature specialization.
 */
template <typename ReturnT, typename... ArgsT>
struct callable_signature<ReturnT (*)(ArgsT...)> {
    using ReturnType   = ReturnT;
    using ArgsTuple    = std::tuple<ArgsT...>;
    using FuncType     = ReturnT (*)(ArgsT...);
    using IsNoexcept_t = std::false_type;

    static constexpr bool IsNoexcept_v = IsNoexcept_t::value;
};

/*!
 * @brief
 * Noexcept free function signature specialization.
 */
template <typename ReturnT, typename... ArgsT>
struct callable_signature<ReturnT (*)(ArgsT...) noexcept> {
    using ReturnType   = ReturnT;
    using ArgsTuple    = std::tuple<ArgsT...>;
    using FuncType     = ReturnT (*)(ArgsT...) noexcept;
    using IsNoexcept_t = std::true_type;

    static constexpr bool IsNoexcept_v = IsNoexcept_t::value;
};

} // namespace chevron::func::traits

#endif // CHEVRON_LIB_HDR_FUNCTION_TYPE_TRAITS_H_
