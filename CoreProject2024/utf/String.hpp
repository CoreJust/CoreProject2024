// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	String.hpp by CoreJust
*	Created on 19.07.2024
*	Contains a number of auxiliary definitions for String type (which uses std::string, but can contain unicode characters).
*/

#pragma once
#include <string>

namespace utf {
	// Utf-8 string, string view and character respectively.
	using String = std::string;
	using StringView = std::string_view;
	using Char = char32_t;
}
