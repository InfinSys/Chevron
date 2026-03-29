
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file func_args.hpp
 *
 * @brief
 */

#ifndef CHEVRON_LIB_HDR_FUNCTION_ARGUMENTS_H_
#define CHEVRON_LIB_HDR_FUNCTION_ARGUMENTS_H_

#include <tuple>
#include <type_traits>
#include <utility>
#include "chevron/utility/function/concepts.hpp"
#include "chevron/common/export.h"

namespace chevron
{

/*!
 * @brief
 * Function arguments container.
 *
 * @details
 * N/A
 */
template <typename... ArgsT>
class CHEVRON_API FuncArgs {
// ===================================================================================== //
//      <> chevron::FuncArgs | TYPE ALIASES
// ===================================================================================== //
  public:
    /*! @brief Argument types pack. */
    using Types = std::tuple<ArgsT...>;

// ===================================================================================== //
//      <> chevron::FuncArgs | CONSTRUCTORS / DESTRUCTOR
// ===================================================================================== //

    /*! @brief Default construct all function arguments. */
    constexpr FuncArgs() noexcept((std::is_nothrow_default_constructible_v<ArgsT> && ...))
        requires(std::is_default_constructible_v<ArgsT> && ...)
        : args{}
    {
        //
    }

    /*! @brief Construct function arguments with specified values. */
    template <typename... IncomingArgsT>
        requires(
            sizeof...(IncomingArgsT) == sizeof...(ArgsT) &&
            (std::is_constructible_v<ArgsT, IncomingArgsT &&> && ...) &&
            (!std::is_same_v<std::remove_cvref_t<IncomingArgsT>, FuncArgs> && ...)
        )
    constexpr explicit FuncArgs(IncomingArgsT&&... arg) noexcept(
        (std::is_nothrow_constructible_v<IncomingArgsT> && ...)
    )
        : args{std::forward<IncomingArgsT>(arg)...}
    {
        //
    }

    ~FuncArgs() noexcept = default;

// ===================================================================================== //
//      <> chevron::FuncArgs | [PUBLIC] MEMBER METHODS
// ===================================================================================== //

    /*! @brief Get argument at specified index. */
    template <std::size_t Index>
        requires(model::concepts::is_valid_args_index<Index, ArgsT...>)
    [[nodiscard]] constexpr decltype(auto) get() noexcept
    {
        return std::get<Index>(this->args);
    }

    /*! @brief Get argument at specified index. */
    template <std::size_t Index>
        requires(model::concepts::is_valid_args_index<Index, ArgsT...>)
    [[nodiscard]] constexpr decltype(auto) get() const noexcept
    {
        return std::get<Index>(this->args);
    }

    /*! @brief Returns pre-defined arguments tuple. */
    [[nodiscard]] constexpr std::tuple<ArgsT...>& argsTuple() noexcept
    {
        return this->args;
    }

    /*! @brief Returns pre-defined arguments tuple. */
    [[nodiscard]] constexpr const std::tuple<ArgsT...>& argsTuple() const noexcept
    {
        return this->args;
    }

// ===================================================================================== //
//      <> chevron::FuncArgs | [PRIVATE] ATTRIBUTES
// ===================================================================================== //
  private:
    /*! @brief Pre-defined arguments tuple. */
    Types args;
};

// ===================================================================================== //
//      <> chevron::FuncArgs | TYPE TRAIT UTILITIES
// ===================================================================================== //

namespace model::traits
{

/*! @brief Function arguments structure conversion utility. */
template <typename T>
struct to_funcargs;

/*! @brief Tuple to function arguments structure specialization. */
template <typename... ArgsT>
struct to_funcargs<std::tuple<ArgsT...>> {
    using type = FuncArgs<ArgsT...>;
};

/*! @brief Function arguments structure conversion utility helper. */
template <typename T>
using to_funcargs_t = to_funcargs<T>::type;

} // namespace model::traits

// ===================================================================================== //
//      <> chevron::FuncArgs | DEDUCTION GUIDES
// ===================================================================================== //

/*! @details Function argument deduction guide. */
template <typename... ArgsT>
FuncArgs(ArgsT&&...) -> FuncArgs<std::decay_t<ArgsT>...>;

/*! @details Empty function argument deduction guide. */
FuncArgs() -> FuncArgs<>;

} // namespace chevron

#endif // CHEVRON_LIB_HDR_FUNCTION_ARGUMENTS_H_
