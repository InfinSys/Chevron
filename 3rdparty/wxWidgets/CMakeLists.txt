
#===============================================
#     /3rdparty/wxWidgets Directory Script
#===============================================

message(STATUS "Fetching wxWidgets library source...")

if(Chevron_SHARED_BUILD)
    # Build wxWidgets as a shared/dynamic library
    set(wxBUILD_SHARED ON)
else()
    # Build wxWidgets as a static library
    set(wxBUILD_SHARED OFF)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Use Microsoft Edge webview backend
    set(wxUSE_WEBVIEW_EDGE ON)
endif()

include(FetchContent)
FetchContent_Declare(
    wxWidgets

    GIT_REPOSITORY
    https://github.com/wxWidgets/wxWidgets.git

    GIT_TAG
    v3.2.9

    GIT_SHALLOW
    ON
)

FetchContent_MakeAvailable(wxWidgets)
