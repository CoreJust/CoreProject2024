// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	TextPosition.hpp by CoreJust
*	Created on 20.07.2024
*	Contains auxiliary types and functions to work with a position (in the format of line:character) in the file.
*/

#pragma once
#include "utf/String.hpp"

namespace utils {
	// Represents a position in a text file.
	struct TextPosition {
		uint32_t line = 0;
		uint32_t character = 0;

		// Returns the substring of a text that corresponds to this text selection.
		// If allowMultipleLines if false, then no more then one (first) line will be added.
		// If empasize is set to true, then escape sequences are applied to colorize the error text.
		// If the selection is out of text, returns an empty string.
		utf::String getTextSelection(utf::StringView text, uint64_t selectionLength, bool allowMultipleLines = true, bool empasize = true) const;

		// Finds the index of the first character of selection in the text based on the TextPosition.
		// (Goes through the given number of lines and on the required line goes to the character).
		// Returns utf::String::npos if the TextPosition is out of the text.
		static uint64_t getIndexInText(TextPosition position, utf::StringView text);

		// Returns text position in a string representatio "line:character".
		utf::String toString() const;
	};
}