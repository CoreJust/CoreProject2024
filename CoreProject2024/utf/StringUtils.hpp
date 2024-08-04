// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	StringUtils.hpp by CoreJust
*	Created on 29.07.2024
*	Contains some auxiliary functions to work with strings.
*/

#pragma once
#include "String.hpp"

namespace utf {
	template<class T>
	concept HasStdToString = requires (T t) {
		{ std::to_string(t) } -> std::convertible_to<utf::String>;
	};

	template<class T>
	concept HasToStringMethod = requires (T t) {
		{ t.toString() } -> std::convertible_to<utf::String>;
	};

	template<class T>
	concept HasToStringMethodViaArrow = requires (T t) {
		{ t->toString() } -> std::convertible_to<utf::String>;
	};

	// Replaces all the characters that change the output flow (such as new lines) with the escape sequences.
	utf::String replaceControlCharacters(utf::StringView origin);

	// Auxiliary function that converts an object to String.
	template<class T>
	utf::String toString(T&& t) {
		if constexpr (std::is_convertible_v<T, utf::String>) {
			return utf::String(t);
		} else if constexpr (HasStdToString<T>) {
			return std::to_string(t);
		} else if constexpr (HasToStringMethod<T>) {
			return t.toString();
		} else if constexpr (HasToStringMethodViaArrow<T>) {
			return t->toString();
		} else {
			static_assert(std::is_convertible_v<T, utf::String> || HasStdToString<T> || HasToStringMethod<T>, "The passed type has neither toString method nor can be converted with std::to_string.");
		}
	}
}