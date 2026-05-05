
#========================================
#     /src/process Directory Script
#========================================

target_sources(
    ${CHEVRON_MAIN_BINARY_NAME}

    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/memory/mem_pool_config.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/memory/proc_allocator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/memory/thread_mem_cache.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/memory/proc_mem_pool.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/memory/memory_core.cpp"

    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/thread/thread_engine.cpp"
)

if(CHEVRON_CXX_GCC OR CHEVRON_CXX_CLANG)
    target_compile_options(
        ${CHEVRON_MAIN_BINARY_NAME}

        PUBLIC
          -march=native  # Optimize for the host CPU
          -mcx16         # Enable 128-bit atomic CAS (x86-64)
    )
endif()
