
# Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
# Released under the terms of the GNU Affero General Public License version 3

# [ISJTB-CXX-XL20260108-000003]

#=======================================================
#
#           Copyright (C) 2026 InfinSys, LLC.
#                  and Jamon Bailey
#
#     Chevron | Application Runtime Infrastructure
#             - Library Build Generation
#
#            (Root CMakeLists.txt Script)
#
#=======================================================

cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(
    chevron_core
    VERSION 0.0.0
    LANGUAGES
        C CXX
)

# Declare project CMake options
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/utility/options.cmake")
# Define platform architecture identifiers
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/platform/detective.cmake")

if (${CMAKE_PROJECT_NAME}_IS_TOP_LEVEL)
    # Export compilation commands to build dir
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    output_host_summary()

    # Include root CML helper module
    include(cmake/utility/root_setup.cmake)
    setup_testing_dependencies()
    setup_clang_format_target()
    setup_clang_tidy_target()
endif()

set(CHEVRON_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(CHEVRON_CMAKE_MODULES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CHEVRON_LIBS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/include")

# Include dependency setup helpers
include("${CHEVRON_CMAKE_MODULES_DIR}/utility/dependencies.cmake")
setup_dependencies()  # Prepare library dependency targets

# Prepare compiler configuration targets
include(cmake/compiler/config_dispatch.cmake)

# Include native install directory variables
include(GNUInstallDirs)

# Source subdirs
add_subdirectory(lib)
add_subdirectory(src)

if (${CMAKE_PROJECT_NAME}_IS_TOP_LEVEL)
    # Testing subdirs
    add_subdirectory(tests)
endif()
