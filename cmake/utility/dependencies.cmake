
#===============================================
#     PROJECT DEPENDENCY MANAGEMENT MODULE
#===============================================

# This script should be invoked from the root
# CMakeLists.txt file.

# Setup project third-party dependencies
macro(setup_dependencies)
    setup_wxwidgets_dependency()
endmacro()

# Setup wxWidgets dependency
macro(setup_wxwidgets_dependency)
    # Check if we are using wxWidgets
    if(Chevron_WX)
        # wxWidgets library source fetch
        add_subdirectory(3rdparty/wxWidgets)

        # wxWidgets library target
        add_library(wxwidgets_external INTERFACE)

        target_link_libraries(
            wxwidgets_external

            INTERFACE
            wxbase
            wxcore
            wxwebview
        )
    endif()
endmacro()
