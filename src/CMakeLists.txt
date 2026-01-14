
#================================
#     /src Directory Script
#================================

# Library main target
if(Chevron_SHARED_BUILD)
    add_library(${CHEVRON_MAIN_BINARY_NAME} SHARED)
else()
    add_library(${CHEVRON_MAIN_BINARY_NAME} STATIC)
endif()

target_include_directories(
    ${CHEVRON_MAIN_BINARY_NAME}

    PUBLIC
    "${CHEVRON_SOURCE_DIR}/include"
    "${CHEVRON_LIBS_INCLUDE_DIR}"
)

# Source directories
add_subdirectory(model)

# Conditionally link against compiler configuration
if(${CHEVRON_CXX_GCC})
    target_link_libraries(
        ${CHEVRON_MAIN_BINARY_NAME}
    
        PRIVATE
        CHEVRON_gcc_cxx_bundle
        CHEVRON_gcc_c_defines
    )
elseif(${CHEVRON_CXX_CLANG})
    target_link_libraries(
        ${CHEVRON_MAIN_BINARY_NAME}
    
        PRIVATE
        CHEVRON_clang_cxx_bundle
        CHEVRON_clang_c_defines
    )
elseif(${CHEVRON_CXX_MSVC})
    target_link_libraries(
        ${CHEVRON_MAIN_BINARY_NAME}
    
        PRIVATE
        CHEVRON_msvc_cxx_bundle
    )
endif()

# Setup installation of Chevron library
install(
    TARGETS
    ${CHEVRON_MAIN_BINARY_NAME}

    COMPONENT CHEVRON
    EXPORT ${CHEVRON_MAIN_BINARY_NAME}-targets

    RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR}
)
