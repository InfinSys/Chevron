
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file func_pointer.hpp
 *
 * @brief
 * Provides declaration of callable function pointer.
 */

#ifndef CHEVRON_LIB_H_FUNCTION_POINTER_H_
#define CHEVRON_LIB_H_FUNCTION_POINTER_H_

#include <functional>
#include "chevron/common/export.hpp"
#include "chevron/model/function/func_args.hpp"

namespace chevron::model
{

/*! @details Base specialization template. */
template <typename ReturnT, typename ArgsClass = FuncArgs<>>
class FuncPtr;

/*!
 * @brief
 * Function pointer.
 *
 * @details
 * N/A
 */
template <typename ReturnT, typename... ArgsT>
class FuncPtr<ReturnT, FuncArgs<ArgsT...>> {
  public:
    /*! @brief Function return type. */
    using ReturnType = ReturnT;
    /*! @brief Function arguments container. */
    using Arguments = FuncArgs<ArgsT...>;
    /*! @brief Specialized standard library function pointer type. */
    using StdFunc = std::function<ReturnT(ArgsT...)>;

    FuncPtr() noexcept = default;

    /*! @brief Construct from a free function, lambda, or functor. */
    template <typename Function>
        requires std::is_invocable_r_v<ReturnType, Function, ArgsT...>
    explicit FuncPtr(Function&& callable) : funcPtr{std::forward<Function>(callable)}
    {
        //
    }

    /*! @brief Construct from a member method of a class instance. */
    template <typename ClassType, typename Method>
    FuncPtr(ClassType* instance, Method method)
    {
        if (instance && method) {
            this->funcPtr = [instance, method](ArgsT... args) -> ReturnType
            {
                return (instance->*method)(std::forward<ArgsT>(args)...);
            };
        }
    }

    ~FuncPtr() noexcept = default;

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
    [[nodiscard]] bool bind(ReturnType(*callable)(ArgsT...))
    {
        if (callable) {
            this->funcPtr = [callable](ArgsT... args) -> ReturnType {
                return callable(std::forward<ArgsT>(args)...);
            };
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
            this->funcPtr = [instance, method](ArgsT... args) -> ReturnType {
                return (instance->*method)(std::forward<ArgsT>(args)...);
            };
        }
        return false;
    }

  private:
    /*! @brief Function pointer. */
    StdFunc funcPtr;
};

/*! @details Default constructor deduction guide. */
//FuncPtr() -> FuncPtr<void, FuncArgs<>>;

/*! @details Free function deduction guide. */
template <typename ReturnT, typename... ArgsT>
FuncPtr(ReturnT (*)(ArgsT...)) -> FuncPtr<ReturnT, FuncArgs<ArgsT...>>;

/*! @details Non-const member function deduction guide. */
template <typename ClassType, typename ReturnT, typename... ArgsT>
FuncPtr(ClassType*, ReturnT (ClassType::*)(ArgsT...)) -> FuncPtr<ReturnT, FuncArgs<ArgsT...>>;

/*! @details Const member function deduction guide. */
template <typename ClassType, typename ReturnT, typename... ArgsT>
FuncPtr(ClassType*, ReturnT (ClassType::*)(ArgsT...) const)
    -> FuncPtr<ReturnT, FuncArgs<ArgsT...>>;

// TODO: Lambda and functor deduction guide!!!

} // namespace chevron::model

#endif // CHEVRON_LIB_H_FUNCTION_POINTER_H_
