
#==================================
#     /tests Directory Script
#==================================

# Enable CTest test registration
enable_testing()

include("${CHEVRON_CMAKE_MODULES_DIR}/utility/testing.cmake")

if(CHEVRON_CLI_DEBUG)
    add_subdirectory(cli)
endif()

if(CHEVRON_UNIT_TEST)
    add_subdirectory(unit)
endif()

if(CHEVRON_BENCHMARK)
    add_subdirectory(benchmark)
endif()
