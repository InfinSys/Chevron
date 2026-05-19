
#=================================================
#     HOST PLATFORM DETECTION SUMMARY MODULE
#=================================================

# ------------------------------------------------
# Descriptive Variable Construction
# ------------------------------------------------

if(CHEVRON_ARCH_64BIT)
    set(_chevron_summary_bitness "64-bit")
elseif(CHEVRON_ARCH_32BIT)
    set(_chevron_summary_bitness "32-bit")
endif()

if(CHEVRON_ARCH_LITTLE_ENDIAN)
    set(_chevron_summary_endianness "little-endian")
elseif(CHEVRON_ARCH_BIG_ENDIAN)
    set(_chevron_summary_endianness "big-endian")
endif()

if(CHEVRON_OS_POSIX)
    set(_chevron_summary_os_suffix " (POSIX)")
else()
    set(_chevron_summary_os_suffix "")
endif()

# ------------------------------------------------
# Summary Emission
# ------------------------------------------------

message(STATUS "")
message(STATUS "[ Chevron Platform Detection ]")
message(STATUS "  Architecture: ${CHEVRON_ARCH_NAME} (${_chevron_summary_bitness}, ${_chevron_summary_endianness})")
message(STATUS "  OS: ${CHEVRON_OS_NAME}${_chevron_summary_os_suffix}")
message(STATUS "")

# ------------------------------------------------
# Scope Cleanup
# ------------------------------------------------

unset(_chevron_summary_bitness)
unset(_chevron_summary_endianness)
unset(_chevron_summary_os_suffix)
