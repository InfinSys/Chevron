
#============================================
#     OPERATING SYSTEM DETECTION MODULE
#============================================

# This script should be invoked from the root
# CMakeLists.txt file.

# -------------------------------------------
# Operating System Identification
# -------------------------------------------

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Linux-Based Operating System
    set(CHEVRON_OS_LINUX ON CACHE INTERNAL "Target operating system")
    set(CHEVRON_OS_POSIX ON CACHE INTERNAL "POSIX-compliant operating system")
    set(CHEVRON_OS_NAME "Linux" CACHE INTERNAL "Target operating system name")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # macOS Operating System
    set(CHEVRON_OS_MACOS ON CACHE INTERNAL "Target operating system")
    set(CHEVRON_OS_POSIX ON CACHE INTERNAL "POSIX-compliant operating system")
    set(CHEVRON_OS_NAME "macOS" CACHE INTERNAL "Target operating system name")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Windows Operating System
    set(CHEVRON_OS_WINDOWS ON CACHE INTERNAL "Target operating system")
    set(CHEVRON_OS_NAME "Windows" CACHE INTERNAL "Target operating system name")
else()
    message(
        FATAL_ERROR
        "Chevron: unrecognized target operating system (CMAKE_SYSTEM_NAME='"
        "${CMAKE_SYSTEM_NAME}'). Extend cmake/platform/operating_sys.cmake "
        "with an associated handler."
    )
endif()
