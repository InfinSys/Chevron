
#=============================================
#     GCC C COMPILER CONFIGURATION (gcc)
#=============================================

# TODO: Setup gcc compiler configuration (if applicable)

set(CHEVRON_GCC_C_VERSION_MIN 13 CACHE STRING "Minimum gcc compiler version")

# Confirm supported gcc compiler version
if(CMAKE_C_COMPILER_VERSION VERSION_LESS ${CHEVRON_GCC_C_VERSION_MIN})
    message(FATAL_ERROR "Incompatible version of GCC C compiler for ${CMAKE_PROJECT_NAME}.")
endif()

# Compiler feature configuration target
add_library(CHEVRON_gcc_c_features INTERFACE)
# Compiler diagnostics configuration target
add_library(CHEVRON_gcc_c_warnings INTERFACE)
# General compiler configuration target
add_library(CHEVRON_gcc_c_options INTERFACE)
# Preprocessor definitions target
add_library(CHEVRON_gcc_c_defines INTERFACE)

# Set C standard version
target_compile_features(
    CHEVRON_gcc_c_features

    INTERFACE
        c_std_11
)

# Set compiler warning flags
target_compile_options(
    CHEVRON_gcc_c_warnings

    INTERFACE
        # Compiler warning flags
        "-Wall"
        "-Wextra"
        "-Wpedantic"
)

# Conditionally add more aggressive warnings
if(CHEVRON_STRICT_C_WARNINGS)
    target_compile_options(
        CHEVRON_gcc_c_warnings

        INTERFACE
            "-Werror"
            "-Wshadow"
            "-Wdouble-promotion"
            "-Wformat=2"
    )
endif()

# Conditionally set compiler optimization level
target_compile_options(
    CHEVRON_gcc_c_options

    INTERFACE
        # Disable optimization on debug
        $<$<CONFIG:Debug>:
            "-O0"
        >

        # High optimization on release
        $<$<CONFIG:Release>:
            "-O2"
        >
)

# Define preprocessor definitions
target_compile_definitions(
    CHEVRON_gcc_c_defines

    INTERFACE
        # Unconditional preprocessor definitions
        CHEVRON_GCC
)

# Complete GCC C compiler package
add_library(CHEVRON_gcc_c_bundle INTERFACE)

target_link_libraries(
    CHEVRON_gcc_c_bundle
    
    INTERFACE
        CHEVRON_gcc_c_features
        CHEVRON_gcc_c_warnings
        CHEVRON_gcc_c_options
        CHEVRON_gcc_c_defines
        CHEVRON_global_c_defines
)
