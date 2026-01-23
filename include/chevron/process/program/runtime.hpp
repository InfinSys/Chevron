
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
 * @file runtime.hpp
 *
 * @brief
 * Declaration of process-level graphical user interface
 * application runtime.
 */

#ifndef CHEVRON_CORE_LIB_PROC_GUI_RUNTIME_H_
#define CHEVRON_CORE_LIB_PROC_GUI_RUNTIME_H_

namespace chevron::process
{

class ProgInstance {
public:
    ProgInstance() noexcept = default;
    ~ProgInstance() noexcept = default;
};

} // namespace chevron::process

#endif // CHEVRON_CORE_LIB_PROC_GUI_RUNTIME_H_
