// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	Concept.hpp by CoreJust
*	Created on 25.07.2024
*	Contains some usefull concepts that are not contained in the standard C++ library.
*/

#pragma once
#include <concepts>

// Arithmetic type is any of the basic integral or floating point types.
template<class T>
concept Arithmetic = std::is_arithmetic_v<T>;

// Callable type is anything that can be called with the given argument types and return something convertible to the required return type.
template<class T, class Result, class... Args>
concept Callable = requires(T func, Args... args) {
	{ func(args...) } -> std::convertible_to<Result>;
};