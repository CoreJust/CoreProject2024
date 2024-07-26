// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	Macro.hpp by CoreJust
*	Created on 25.07.2024
*	Contains a number of auxiliary macros.
*/

#pragma once

#if defined(__cpp_lib_unreachable)
#include <utility> // for std::unreachable()
#endif

#undef INLINE
#undef NO_UNIQUE_ADDRESS
#undef LIKELY
#undef UNLIKELY
#undef LANG_VERSION

// LANG_VERSION - the version of c++
#if defined(_MSVC_LANG) && _MSVC_LANG > __cplusplus
#define LANG_VERSION _MSVC_LANG
#else
#define LANG_VERSION __cplusplus
#endif

// Self-explanatory
#if LANG_VERSION < 202002L
#error "C++ language version must be c++20 or higher"
#endif

// Define unreachable, must be either clang, gcc, intel, or msvc
#if defined(__cpp_lib_unreachable)
#define unreachable() std::unreachable()
#elif defined(__clang__) || defined(__GNUC__) || defined(__INTEL_COMPILER)
#define unreachable() __builtin_unreachable()
#elif defined(_MSC_VER)
#define unreachable() __assume(false)
#else
#error "Unsupported compiler"
#endif

#if defined(_MSC_VER)
#define NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
#define NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif

// If [[likely]] and [[unlikely]] is not supported - delete them
#if !defined(__has_cpp_attribute)
#define LIKELY
#define UNLIKELY
#elif !(__has_cpp_attribute(likely) && __has_cpp_attribute(unlikely))
#define LIKELY
#define UNLIKELY
#else
#define LIKELY [[likely]]
#define UNLIKELY [[unlikely]]
#endif

#if defined(_MSC_VER)
#define INLINE __forceinline
#else
#define INLINE __attribute__((always_inline))
#endif