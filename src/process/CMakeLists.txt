
#========================================
#     /src/process Directory Script
#========================================

target_sources(
    ${CHEVRON_MAIN_BINARY_NAME}

    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/memory/mem_pool_config.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/memory/proc_allocator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/memory/proc_mem_pool.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/memory/memory_core.cpp"

    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/thread/thread_engine.cpp"
)
