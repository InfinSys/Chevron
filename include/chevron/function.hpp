
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

#include "chevron/utility/function/func_args.hpp"
#include "chevron/utility/function/callable.hpp"
#include <string>

namespace chevron
{

/*! @brief Callable function that accepts no arguments and returns void. */
using VoidReturnNoArgs = Callable<void>;

/*! @brief Callable function that accepts no arguments and returns a boolean. */
using BoolReturnNoArgs = Callable<bool>;

/*! @brief Callable function that accepts no arguments and returns a character. */
using CharReturnNoArgs = Callable<char>;

/*! @brief Callable function that accepts no arguments and returns an integer. */
using IntReturnNoArgs = Callable<int>;

/*! @brief Callable function that accepts no arguments and returns a float. */
using FloatReturnNoArgs = Callable<float>;

/*! @brief Callable function that accepts no arguments and returns a double. */
using DoubleReturnNoArgs = Callable<double>;

/*! @brief Callable function that accepts no arguments and returns a const char pointer. */
using CstrReturnNoArgs = Callable<const char*>;

/*! @brief Callable function that accepts no arguments and returns a string. */
using StringReturnNoArgs = Callable<std::string>;

/*! @brief Callable function that accepts no arguments and returns size_t. */
using SizeReturnNoArgs = Callable<size_t>;

/*! @brief Callable function that accepts no arguments and returns specified type. */
template <typename ReturnT>
using NoArgsReturn = Callable<ReturnT>;

} // namespace chevron

#endif // CHEVRON_LIB_HDR_FUNCTION_INCLUDE_H_
