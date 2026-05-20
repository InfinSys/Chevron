
#=================================================
#     PLATFORM DETECTION COORDINATION MODULE
#=================================================

# This script should be invoked from the root
# CMakeLists.txt file.

include(cmake/platform/architecture.cmake)
include(cmake/platform/operating_sys.cmake)
include(cmake/platform/cpu_features.cmake)

# Output summary of build target platform
macro(output_target_platform_summary)
    include(cmake/utility/target_summary.cmake)
endmacro()
