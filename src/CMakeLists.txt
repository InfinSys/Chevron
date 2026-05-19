
#================================
#     /src Directory Script
#================================

# Library main target
if(Chevron_SHARED_BUILD)
    add_library(${CHEVRON_MAIN_BINARY_NAME} SHARED)
else()
    add_library(${CHEVRON_MAIN_BINARY_NAME} STATIC)
endif()

# Set library package version properties
set_target_properties(
    ${CHEVRON_MAIN_BINARY_NAME} PROPERTIES

    SOVERSION
      ${CMAKE_PROJECT_VERSION_MAJOR}

    VERSION
      ${CMAKE_PROJECT_VERSION}
)

target_include_directories(
    ${CHEVRON_MAIN_BINARY_NAME}

    PUBLIC
      "${CHEVRON_SOURCE_DIR}/include"
      "${CHEVRON_LIBS_INCLUDE_DIR}"
)

# Source directories
add_subdirectory(process)
add_subdirectory(memory)

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

if(Chevron_WX)
    # Link to wxWidgets library
    target_link_libraries(
        ${CHEVRON_MAIN_BINARY_NAME}

        PUBLIC
          wxwidgets_external
    )
endif()

# Setup installation of Chevron library
include("${CHEVRON_CMAKE_MODULES_DIR}/install_config.cmake")
