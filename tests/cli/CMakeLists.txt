
#======================================
#     /tests/cli Directory Script
#======================================

# Create developer command line executable
add_executable(devexe "dev_exe.cpp")

target_link_libraries(
    devexe

    PRIVATE
      ${CHEVRON_MAIN_BINARY_NAME}
)

if(${CHEVRON_CXX_GCC})
    target_link_libraries(
        devexe
    
        PRIVATE
        CHEVRON_gcc_cxx_bundle
    )
elseif(${CHEVRON_CXX_CLANG})
    target_link_libraries(
        devexe
    
        PRIVATE
        CHEVRON_clang_cxx_bundle
    )
elseif(${CHEVRON_CXX_MSVC})
    target_link_libraries(
        devexe
    
        PRIVATE
        CHEVRON_msvc_cxx_bundle
    )
endif()

# Setup installation of developer executable
install(
    TARGETS devexe
    COMPONENT DevDebugApp
    EXPORT devexe-targets

    RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR}
)
