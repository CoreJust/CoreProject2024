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
	// Replaces all the characters that change the output flow (such as new lines) with the escape sequences.
	utf::String replaceControlCharacters(utf::StringView origin);
}