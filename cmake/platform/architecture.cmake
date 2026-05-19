
#===============================================
#     TARGET ARCHITECTURE DETECTION MODULE
#===============================================

# This script should be invoked from the root
# CMakeLists.txt file.

# ----------------------------------------------
# Architecture Identification
# ----------------------------------------------

string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" _chevron_arch_processor)

if(_chevron_arch_processor MATCHES "^(amd64|x86_64|x64)$")
    # x86-64 Based Processor
    set(CHEVRON_ARCH_X86 ON CACHE INTERNAL "Target architecture family")
    set(CHEVRON_ARCH_X86_64 ON CACHE INTERNAL "Target architecture")
    set(CHEVRON_ARCH_NAME "x86_64" CACHE INTERNAL "Target architecture name")
    set(_chevron_arch_expected_bits 64)
elseif(_chevron_arch_processor MATCHES "^(i[3-6]86|x86)$")
    # x86 Based Processor
    set(CHEVRON_ARCH_X86 ON CACHE INTERNAL "Target architecture family")
    set(CHEVRON_ARCH_X86_32 ON CACHE INTERNAL "Target architecture")
    set(CHEVRON_ARCH_NAME "x86" CACHE INTERNAL "Target architecture name")
    set(_chevron_arch_expected_bits 32)
elseif(_chevron_arch_processor MATCHES "^(aarch64|arm64)$")
    # ARM64 Based Processor
    set(CHEVRON_ARCH_ARM ON CACHE INTERNAL "Target architecture family")
    set(CHEVRON_ARCH_ARM64 ON CACHE INTERNAL "Target architecture")
    set(CHEVRON_ARCH_NAME "arm64" CACHE INTERNAL "Target architecture name")
    set(_chevron_arch_expected_bits 64)
elseif(_chevron_arch_processor MATCHES "^(armv[5-8].*|arm)$")
    # ARM32 Based Processor
    set(CHEVRON_ARCH_ARM ON CACHE INTERNAL "Target architecture family")
    set(CHEVRON_ARCH_ARM32 ON CACHE INTERNAL "Target architecture")
    set(CHEVRON_ARCH_NAME "arm32" CACHE INTERNAL "Target architecture name")
    set(_chevron_arch_expected_bits 32)
elseif(_chevron_arch_processor MATCHES "^riscv64$")
    # RISC-V 64 Based Processor
    set(CHEVRON_ARCH_RISCV ON CACHE INTERNAL "Target architecture family")
    set(CHEVRON_ARCH_RISCV64 ON CACHE INTERNAL "Target architecture")
    set(CHEVRON_ARCH_NAME "riscv64" CACHE INTERNAL "Target architecture name")
    set(_chevron_arch_expected_bits 64)
elseif(_chevron_arch_processor MATCHES "^riscv32$")
    # RISC-V 32 Based Processor
    set(CHEVRON_ARCH_RISCV ON CACHE INTERNAL "Target architecture family")
    set(CHEVRON_ARCH_RISCV32 ON CACHE INTERNAL "Target architecture")
    set(CHEVRON_ARCH_NAME "riscv32" CACHE INTERNAL "Target architecture name")
    set(_chevron_arch_expected_bits 32)
else()
    message(
        FATAL_ERROR
        "Chevron: unrecognized target architecture '${CMAKE_SYSTEM_PROCESSOR}'. "
        "Extend 'cmake/platform/architecture.cmake' with a matching pattern and "
        "the appropriate architecture hints.\n"
    )
endif()

# ----------------------------------------------
# Processor Bitness
# ----------------------------------------------

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CHEVRON_ARCH_64BIT ON CACHE INTERNAL "64-bit architecture")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(CHEVRON_ARCH_32BIT ON CACHE INTERNAL "32-bit architecture")
else()
    message(
        FATAL_ERROR
        "Chevron: unsupported target pointer size. CMAKE_SIZEOF_VOID_P= "
        "'${CMAKE_SIZEOF_VOID_P}', but Chevron only supports 32-bit "
        "(4-byte) and 64-bit (8-byte) platforms only.\n"
    )
endif()

if(_chevron_arch_expected_bits EQUAL 64 AND NOT CHEVRON_ARCH_64BIT)
    message(
        FATAL_ERROR
        "Chevron: architecture/bitness mismatch. Detected '${CHEVRON_ARCH_NAME}' "
        "but CMAKE_SIZEOF_VOID_P reports a non-64-bit pointer size "
        "(${CMAKE_SIZEOF_VOID_P}-byte).\n"
    )
elseif(_chevron_arch_expected_bits EQUAL 32 AND NOT CHEVRON_ARCH_32BIT)
    message(
        FATAL_ERROR
        "Chevron: architecture/bitness mismatch. Detected '${CHEVRON_ARCH_NAME}' "
        "but CMAKE_SIZEOF_VOID_P reports a non-32-bit pointer size "
        "(${CMAKE_SIZEOF_VOID_P}-byte).\n"
    )
endif()

# ----------------------------------------------
# Architecture Endianness
# ----------------------------------------------

if(CMAKE_CXX_BYTE_ORDER STREQUAL "LITTLE_ENDIAN")
    set(CHEVRON_ARCH_LITTLE_ENDIAN ON CACHE INTERNAL "Little-endian architecture")
elseif(CMAKE_CXX_BYTE_ORDER STREQUAL "BIG_ENDIAN")
    set(CHEVRON_ARCH_BIG_ENDIAN ON CACHE INTERNAL "Big-endian architecture")
else()
    message(
        FATAL_ERROR
        "Chevron: unable to determine target byte order. CMAKE_CXX_BYTE_ORDER="
        "'${CMAKE_CXX_BYTE_ORDER}'.\n"
    )
endif()

# ----------------------------------------------
# Scope Cleanup
# ----------------------------------------------

unset(_chevron_arch_processor)
unset(_chevron_arch_expected_bits)
