
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
  * @file scope_guard.hpp
  *
  * @brief
  * RAII utility binding an arbitrary cleanup process to scope
  * exit.
  *
  * @author
  * Jamon T. Bailey
  *
  * @date 04-30-2026
  */

#ifndef CHEVRON_LIB_HDR_SCOPE_RESOURCE_GUARD_H_
#define CHEVRON_LIB_HDR_SCOPE_RESOURCE_GUARD_H_

#include <type_traits>
#include "chevron/common/macro_defs.h"

#if CHEVRON_MSVC
  #define NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#elif CHEVRON_GCC || CHEVRON_CLANG
  #define NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif

namespace chevron::utility
{

// TODO: NEED TO ENFORCE VOID RETURN LAMBDA HERE!!!

/*!
 * @brief
 * RAII scope guard that invokes a callable on scope exit.
 * 
 * @warning
 * Marked `[[nodiscard]]` to prevent unnamed temporaries
 * that would execute immediately on construction and
 * destruction, defeating the purpose of guarding a scope.
 * 
 * @details
 * Holds an unnamed callable and an active flag. If the flag
 * still indicates active status on scope exit, the provided
 * unnamed callable is invoked within the destructor of this
 * type. This automates scope-exiting tasks. The execution of
 * the provided callable can be disarmed by calling `dismiss()`
 * before the scope that defined the guard exits. The move
 * constructor transfers the obligation to a new guard and
 * disarms the original source.
 */
template <typename LambdaT>
class [[nodiscard]] ScopeGuard {
	// ===================================================================================== //
    //      <> chevron::utility::ScopeGuard | CONSTRUCTORS / DESTRUCTOR
    // ===================================================================================== //
public:
	/*!
	 * @brief
	 * Construct scope guard with task on scope exit.
	 */
	[[nodiscard]] explicit ScopeGuard(LambdaT on_exit)
		noexcept(std::is_nothrow_constructible_v<LambdaT>)
		: task_{on_exit},
		active_{true}
	{
		//
	}

	ScopeGuard(const ScopeGuard<LambdaT>&) = delete;

	/*!
	 * @brief
	 * Transfer scope guard obligation to another scope guard.
	 * 
	 * @details
	 * Moves the provided scope guards active state to this new
	 * scope guard while also disarming and invalidating the
	 * original source to ensure the provided unnamed callable
	 * is invoked only once.
	 */
	ScopeGuard(ScopeGuard<LambdaT>&& other)
		noexcept(std::is_nothrow_constructible_v<LambdaT>)
		: task_{std::move(other.task_)},
		active_{other.active_}
	{
		other.active_ = false;
	}

	ScopeGuard() = delete;

	/*!
	 * @brief
	 * Execute scope exit task if guard is still active.
	 * 
	 * @details
	 * Invokes the stored unnamed callable only if the guard
	 * has not been dismissed or moved from. Given that the
	 * callable is invoked within a destructor, it must not
	 * throw.
	 */
	~ScopeGuard() noexcept
	{
		if (active_)
			task_();
	}

	// ===================================================================================== //
    //      <> chevron::utility::ScopeGuard | [PUBLIC] MEMBER METHODS
    // ===================================================================================== //

	/*!
	 * @brief
	 * Relieve scope guard of its obligation.
	 * 
	 * @details
	 * Disarms the guard so the callable is not invoked on
	 * scope exit.
	 */
	void dismiss() noexcept
	{
		active_ = false;
	}

	// ===================================================================================== //
    //      <> chevron::utility::ScopeGuard | OPERATORS
    // ===================================================================================== //

	ScopeGuard<LambdaT>& operator=(const ScopeGuard<LambdaT>&) = delete;

	/*!
	 * @details
	 * The semantics of move-assigning into a still-active
	 * guard are ambiguous: should the destination's pending
	 * task run, or be silently dropped? Rather than choose,
	 * the operation is forbidden. Use move construction at the
	 * point of need instead.
	 */
	ScopeGuard<LambdaT>& operator=(ScopeGuard<LambdaT>&&) = delete;

	// ===================================================================================== //
    //      <> chevron::utility::ScopeGuard | [PRIVATE] ATTRIBUTES
    // ===================================================================================== //
private:
	NO_UNIQUE_ADDRESS LambdaT task_;   ///< Scope exit task callable
	bool active_;                      ///< Scope exit task executaion flag
};

// ===================================================================================== //
//      <> chevron::utility::ScopeGuard | DEDUCTION GUIDES
// ===================================================================================== //

/*! @details Lambda deduction guide. */
template <typename LambdaT>
ScopeGuard(LambdaT) -> ScopeGuard<std::decay_t<LambdaT>>;

}

#undef NO_UNIQUE_ADDRESS

#endif // CHEVRON_LIB_HDR_SCOPE_RESOURCE_GUARD_H_
