
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file function.hpp
 *
 * @brief
 * General include header for function related facilities.
 */

#ifndef CHEVRON_LIB_HDR_FUNCTION_INCLUDE_H_
#define CHEVRON_LIB_HDR_FUNCTION_INCLUDE_H_

#include "chevron/model/function/func_args.hpp"
#include "chevron/model/function/func_pointer.hpp"
#include <string>

namespace chevron
{

/*! @brief Callable function that accepts no arguments and returns void. */
using VoidReturnNoArgs = FuncPtr<void>;

/*! @brief Callable function that accepts no arguments and returns a boolean. */
using BoolReturnNoArgs = FuncPtr<bool>;

/*! @brief Callable function that accepts no arguments and returns a character. */
using CharReturnNoArgs = FuncPtr<char>;

/*! @brief Callable function that accepts no arguments and returns an integer. */
using IntReturnNoArgs = FuncPtr<int>;

/*! @brief Callable function that accepts no arguments and returns a float. */
using FloatReturnNoArgs = FuncPtr<float>;

/*! @brief Callable function that accepts no arguments and returns a double. */
using DoubleReturnNoArgs = FuncPtr<double>;

/*! @brief Callable function that accepts no arguments and returns a const char pointer. */
using CstrReturnNoArgs = FuncPtr<const char*>;

/*! @brief Callable function that accepts no arguments and returns a string. */
using StringReturnNoArgs = FuncPtr<std::string>;

/*! @brief Callable function that accepts no arguments and returns size_t. */
using SizeReturnNoArgs = FuncPtr<size_t>;

/*! @brief Callable function that accepts no arguments and returns specified type. */
template <typename ReturnT>
using NoArgsReturn = FuncPtr<ReturnT>;

} // namespace chevron

#endif // CHEVRON_LIB_HDR_FUNCTION_INCLUDE_H_
