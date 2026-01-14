
#=======================================
#     PROJECT CMAKE OPTIONS MODULE
#=======================================

# This script should be invoked from the root
# CMakeLists.txt file.

option(
    Chevron_SHARED_BUILD
    "Build Chevron as a shared/dynamic library."
    ON
)

option(
    Chevron_WX
    "Build Chevron with a wxWidgets dependency."
    OFF
)

option(
    Chevron_WX_SHARED_BUILD
    "Build Chevron's wxWidgets dependency as a shared/dynamic library."
    ON
)
