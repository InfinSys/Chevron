
#======================================
#     PROJECT INSTALLATION MODULE
#======================================

# This script should be invoked at the end of
# the /src/CMakeLists.txt file.

# Install main library binary
install(
    TARGETS
      ${CHEVRON_MAIN_BINARY_NAME}

    COMPONENT CHEVRON
      EXPORT ${CHEVRON_MAIN_BINARY_NAME}-targets

    RUNTIME
      DESTINATION ${CMAKE_INSTALL_BINDIR}

    LIBRARY
      DESTINATION ${CMAKE_INSTALL_LIBDIR}

    ARCHIVE
      DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
