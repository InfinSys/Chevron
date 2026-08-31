
#=======================================
#     /src/memory Directory Script
#=======================================

target_sources(
    ${CHEVRON_MAIN_BINARY_NAME}

    PUBLIC
      "${CMAKE_CURRENT_SOURCE_DIR}/description.cpp"
      "${CMAKE_CURRENT_SOURCE_DIR}/chunk_descriptor.cpp"
      "${CMAKE_CURRENT_SOURCE_DIR}/region.cpp"
      "${CMAKE_CURRENT_SOURCE_DIR}/atomic_free_list.cpp"
      "${CMAKE_CURRENT_SOURCE_DIR}/scope.cpp"
)
