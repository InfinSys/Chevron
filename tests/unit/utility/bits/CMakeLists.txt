
#===============================================
#     /tests/unit/utility/bits Directory Script
#===============================================

add_unit_test_target(
    NAME bits_unit_tests

    SOURCES
      alignment.test.cpp
      powers.test.cpp
)

target_link_libraries(
    bits_unit_tests

    PRIVATE
      ${CHEVRON_MAIN_BINARY_NAME}
)
