
#========================================
#     PROJECT TESTING HELPER MODULE
#========================================

# --------------------------------------------------------------------
# @note
# Intended to be included from the `tests/CMakeLists.txt` script,
# generally after the `/3rdparty` directory has made GoogleTest and
# Google Benchmark available as imported targets.
#
# @details
# Defines test target creation functions for different categories of
# tests. This includes strategies such as unit tests, stress tests,
# and benchmarks. Each of the public helper function defined here
# encapsulates the process of executable creation, library linking,
# and CTest registration so that `CMakeLists.txt` scripts under the
# `/test` directory are clear, concise, and consistent.
#
# @author Jamon T. Bailey
# @date 05-23-2026
# --------------------------------------------------------------------

include_guard(GLOBAL)

include(GoogleTest)

# ---> Establish GoogleTest interface target <------------------------
add_library(googletest_external INTERFACE)

target_include_directories(
    googletest_external

    INTERFACE
      "${googletest_SOURCE_DIR}/googletest/include"
)

target_link_libraries(
    googletest_external

    INTERFACE
      gtest
      gtest_main
)
# --------------------------------------------------------------------

# ---> Establish Google Benchmark interface target <------------------
add_library(benchmark_external INTERFACE)

target_include_directories(
    benchmark_external

    INTERFACE
      "${benchmark_SOURCE_DIR}/include"
)

target_link_libraries(
    benchmark_external

    INTERFACE
      benchmark
      benchmark_main
)
# --------------------------------------------------------------------

# Create test executable, apply common link dependencies, and register
# the target with CTest via `gtest_discover_tests()`.
function(_add_test_target_base)
    set(single_args NAME)
    set(multi_args SOURCES LINK LABELS)
    cmake_parse_arguments(ARG "" "${single_args}" "${multi_args}" ${ARGN})

    if(NOT ARG_NAME)
        message(
            FATAL_ERROR
            "Name argument is required for the creation of a testing target."
        )
    endif()

    # Create testing executable
    if(NOT ARG_SOURCES)
        # Sources will be provided manually later by caller
        add_executable(${ARG_NAME})
    else()
        add_executable(${ARG_NAME} ${ARG_SOURCES})
    endif()

    # Link against relevant testing libraries
    target_link_libraries(
        ${ARG_NAME}

        PRIVATE
          ${ARG_LINK}
    )

    # Conditionally link against compiler configuration
    if(${CHEVRON_CXX_GCC})
        target_link_libraries(
            ${ARG_NAME}

            PRIVATE
              CHEVRON_gcc_cxx_bundle
              CHEVRON_gcc_c_defines
        )
    elseif(${CHEVRON_CXX_CLANG})
        target_link_libraries(
            ${ARG_NAME}

            PRIVATE
              CHEVRON_clang_cxx_bundle
              CHEVRON_clang_c_defines
        )
    elseif(${CHEVRON_CXX_MSVC})
        target_link_libraries(
            ${ARG_NAME}

            PRIVATE
              CHEVRON_msvc_cxx_bundle
        )
    endif()

    gtest_discover_tests(
        ${ARG_NAME}

        PROPERTIES
          LABELS "${ARG_LABELS}"
    )
endfunction()

# Create unit testing target
function(add_unit_test_target)
    set(single_args NAME)
    set(multi_args SOURCES LABELS)
    cmake_parse_arguments(ARG "" "${single_args}" "${multi_args}" ${ARGN})

    _add_test_target_base(
        NAME ${ARG_NAME}

        SOURCES
          ${ARG_SOURCES}

        LINK
          googletest_external

        LABELS
          unit
          ${ARG_LABELS}
    )
endfunction()

# Create stress/soak testing target
function(add_stress_test_target)
    set(single_args NAME)
    set(multi_args SOURCES LABELS)
    cmake_parse_arguments(ARG "" "${single_args}" "${multi_args}" ${ARGN})

    _add_test_target_base(
        NAME ${ARG_NAME}

        SOURCES
          ${ARG_SOURCES}

        LINK
          googletest_external

        LABELS
          stress
          ${ARG_LABELS}
    )
endfunction()
