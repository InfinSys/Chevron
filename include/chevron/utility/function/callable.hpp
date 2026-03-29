
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file callable.hpp
 *
 * @brief
 * Provides declaration of callable function pointer.
 *
 * @author
 * Jamon T. Bailey
 *
 * @date 01-15-2026
 */

#ifndef CHEVRON_LIB_HDR_FUNCTION_POINTER_H_
#define CHEVRON_LIB_HDR_FUNCTION_POINTER_H_

#include <functional>
#include "chevron/utility/function/func_args.hpp"
#include "chevron/utility/function/type_traits.hpp"

namespace chevron
{

/*! @brief Base specialization template. */
template <typename ReturnT, typename ArgsClass = FuncArgs<>>
class Callable;

/*!
 * @brief
 * Function pointer.
 *
 * @details
 * N/a
 */
template <typename ReturnT, typename... ArgsT>
class Callable<ReturnT, FuncArgs<ArgsT...>> {
    // ===================================================================================== //
    //      <> chevron::Callable | TYPE ALIASES
    // ===================================================================================== //
  public:
    /*! @brief Function return type. */
    using ReturnType = ReturnT;
    /*! @brief Function arguments container. */
    using Arguments = FuncArgs<ArgsT...>;
    /*! @brief Specialized standard library function pointer type. */
    using StdFunc = std::function<ReturnT(ArgsT...)>;

    // ===================================================================================== //
    //      <> chevron::Callable | CONSTRUCTORS / DESTRUCTOR
    // ===================================================================================== //

    /*! @brief Construct from a free function, lambda, or functor. */
    template <typename Function>
        requires std::is_invocable_r_v<ReturnType, Function, ArgsT...>
    explicit Callable(Function&& callable) : funcPtr{std::forward<Function>(callable)}
    {
        //
    }

    /*! @brief Construct from a member method of a class instance. */
    template <typename ClassType, typename Method>
    Callable(ClassType* instance, Method method)
    {
        if (instance && method) {
            this->funcPtr = [instance, method](ArgsT... args) -> ReturnType
            {
                return (instance->*method)(std::forward<ArgsT>(args)...);
            };
        }
    }

    Callable() noexcept = default;

    ~Callable() noexcept = default;

    // ===================================================================================== //
    //      <> chevron::Callable | [PUBLIC] MEMBER METHODS
    // ===================================================================================== //

    /*!
     * @brief
     * Evaluates presence of internal callable function.
     *
     * @return
     * True if no function present
     */
    [[nodiscard]] bool isInvalid() const noexcept
    {
        return !this->operator bool();
    }

    /*!
     * @brief
     * Bind callable free function, lambda, or functor.
     *
     * @return
     * True if successful
     */
    [[nodiscard]] bool bind(ReturnType (*callable)(ArgsT...))
    {
        if (callable) {
            this->funcPtr = std::forward<ReturnType (*)(ArgsT...)>(callable);
            return true;
        }
        return false;
    }

    /*!
     * @brief
     * Bind callable member method of a class.
     *
     * @return
     * True if successful
     */
    template <typename ClassType, typename Method>
    [[nodiscard]] bool bind(ClassType* instance, Method method)
    {
        if (instance && method) {
            this->funcPtr = [instance, method](ArgsT... args) -> ReturnType
            {
                return (instance->*method)(std::forward<ArgsT>(args)...);
            };
            return true;
        }
        return false;
    }

    /* ------------------------------------------------------------------------------------- */
    //      > chevron::Callable | OPERATORS
    /* ------------------------------------------------------------------------------------- */

    /*! @brief Execute function with specified arguments. */
    ReturnType operator()(ArgsT... arg) const
    {
        return this->funcPtr(std::forward<ArgsT>(arg)...);
    }

    /*! @brief Execute function with specified arguments. */
    ReturnType operator()(Arguments& arguments)
    {
        return std::apply(this->funcPtr, arguments.argsTuple());
    }

    /*! @brief Verify presence of callable function. */
    [[nodiscard]] explicit operator bool() const noexcept
    {
        return !!this->funcPtr;
    }

    // ===================================================================================== //
    //      <> chevron::Callable | [PRIVATE] ATTRIBUTES
    // ===================================================================================== //
  private:
    /*! @brief Function pointer. */
    StdFunc funcPtr;
};

// ===================================================================================== //
//      <> chevron::Callable | DEDUCTION GUIDES
// ===================================================================================== //

/*! @details Free function deduction guide. */
template <typename ReturnT, typename... ArgsT>
Callable(ReturnT (*)(ArgsT...)) -> Callable<ReturnT, FuncArgs<ArgsT...>>;

/*! @details Non-const member function deduction guide. */
template <typename ClassType, typename ReturnT, typename... ArgsT>
Callable(ClassType*, ReturnT (ClassType::*)(ArgsT...))
    -> Callable<ReturnT, FuncArgs<ArgsT...>>;

/*! @details Const member function deduction guide. */
template <typename ClassType, typename ReturnT, typename... ArgsT>
Callable(ClassType*, ReturnT (ClassType::*)(ArgsT...) const)
    -> Callable<ReturnT, FuncArgs<ArgsT...>>;

/*! @details Lambda and functor deduction guide. */
template <typename Function>
Callable(Function&&) -> Callable<
    typename model::traits::callable_signature<std::decay_t<Function>>::ReturnType,
    model::traits::to_funcargs_t<
        typename model::traits::callable_signature<std::decay_t<Function>>::ArgsTuple>>;

// TODO: Lambda and functor deduction guide only works
//       when directly naming `Callable` with it's fully
//       qualified name. Other instantiations do not
//       appear to exhibit this behavior in identical
//       circumstances.

// NOTE: Is this because there is no `Callable` constructor
//       that can construct a call to the `::operator()`
//       method of the lambda/functor? Perhaps the sole
//       reliance on the guide to properly make the
//       appropriate type is the issue here.

} // namespace chevron

#endif // CHEVRON_LIB_HDR_FUNCTION_POINTER_H_
