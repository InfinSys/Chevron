
#==================================
#     /tests Directory Script
#==================================

if(CHEVRON_CLI_DEBUG)
    add_subdirectory(cli)
endif()

if(CHEVRON_UNIT_TEST)
    add_subdirectory(unit)
endif()

if(CHEVRON_BENCHMARK)
    add_subdirectory(benchmark)
endif()
