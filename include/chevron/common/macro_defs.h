
// Copyright (C) 2026 by Jamon T. Bailey and InfinSys, LLC. All rights reserved.
// Released under the terms of the GNU Affero General Public License version 3

// [ISJTB-CXX-XL20260108-000003]

/*!
  * @file macro_defs.h
  *
  * @brief
  * Defines library-wide macro definitions.
  *
  * @author
  * Jamon T. Bailey
  *
  * @date 04-22-2026
  */

#ifndef CHEVRON_LIB_HDR_COMMON_MACRO_DEFINITIONS_H_
#define CHEVRON_LIB_HDR_COMMON_MACRO_DEFINITIONS_H_

// ===================================================================================== //
//      [ ARCHITECTURE DETECTION ]
// 
// Identifies the CPU architecture of the target platform. Each supported architecture
// is probed with both its GCC/Clang spelling and its MSVC spelling, since the two
// compiler families do not share a common macro for this.
// ===================================================================================== //

#if defined(__x86_64__) || defined(_M_X64)
  #define CHEVRON_ARCH_X86_64 1
#elif defined(__i386__) || defined(_M_IX86)
  #define CHEVRON_ARCH_X86 1
#elif defined(__aarch64__) || defined(_M_ARM64)
  #define CHEVRON_ARCH_ARM64 1
#elif defined(__arm__) || defined(_M_ARM)
  #define CHEVRON_ARCH_ARM 1
#endif

// ===================================================================================== //
//      [ COMPILER DETECTION ]
// 
// Identifies the compiler building Chevron. Clang is checked before GCC because Clang
// defines `__GNUC__` for GCC compatibility, and MSVC is identified through `_MSC_VER`.
// ===================================================================================== //

#if defined(__clang__)
  #define CHEVRON_CLANG 1
#elif defined(__GNUC__)
  #define CHEVRON_GCC   1
#elif defined(_MSC_VER)
  #define CHEVRON_MSVC  1
#else
  #error "Chevron does not support this compiler."
#endif

// ===================================================================================== //
//      [ OPERATING SYSTEM DETECTION ]
// 
// Identifies the operating system of the target platform. `_WIN32` is defined on both
// 32 and 64-bit Windows and is the canonical Windows marker; `_WIN64` is checked
// alongside it defensively. Cygwin is treated as a distinct branch because it targets
// a POSIX layer on top of Windows rather than the native Win32 API.
// ===================================================================================== //

#if defined(_WIN32) || defined(_WIN64)
  #define CHEVRON_WINDOWS 1
#elif defined(__CYGWIN__)
  #define CHEVRON_CYGWIN 1
#elif defined(__APPLE__)
  #define CHEVRON_MACOS 1
#elif defined(__linux__)
  #define CHEVRON_LINUX 1
#else
  #error "Chevron does not support this operating system."
#endif

// ===================================================================================== //
//      [ POINTER-WIDTH DETECTION ]
// 
// Identifies the target platforms pointer/address width using `UINTPTR_MAX` defined in
// `<stdint.h>`, which represents the maximum value of `uintptr_t`. This approach evades
// the complexities of vendor-specific macros.
// ===================================================================================== //

#include <stdint.h>

#if UINTPTR_MAX == 0xFFFFFFFFFFFFFFFFu
  #define CHEVRON_64BIT 1
  #define CHEVRON_PTR_WIDTH 64
#elif UINTPTR_MAX == 0xFFFFFFFFu
  #define CHEVRON_32BIT 1
  #define CHEVRON_PTR_WIDTH 32
#else
  #error "Chevron only supports 32/64-bit platforms."
#endif

#endif // CHEVRON_LIB_HDR_COMMON_MACRO_DEFINITIONS_H_
