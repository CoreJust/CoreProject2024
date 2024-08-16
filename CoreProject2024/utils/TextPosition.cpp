// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "TextPosition.hpp"
#include <format>
#include "utf/Utf.hpp"

utf::String utils::TextPosition::getTextSelection(utf::StringView text, uint64_t selectionLength, bool allowMultipleLines, bool empasize) const {
	uint64_t selectionStart = getIndexInText(TextPosition { .line = line, .character = 0 }, text);
	if (selectionStart == utf::String::npos) {
		return ""; // this TextPosition is out of text.
	}

	utf::String result;
	if (allowMultipleLines) {
		result.reserve(selectionLength * (1 + bool(empasize)));
	}

	// actualCharacter is character - 1 or 0 if the character is 0, because the first character in the
	// line is new line in the TextPosition representation, but when we want it to print the first character,
	// the first character after the new line character is usually meant.
	uint32_t actualCharacter = character > 0 ? character - 1 : 0;

	int64_t charactersLeft = static_cast<int64_t>(selectionLength + actualCharacter);
	const char* ptr = text.data() + selectionStart;
	const char* end = text.data() + text.size();
	int64_t charactersTillEmphasizeStart = actualCharacter;

	while (ptr < end) {
		utf::Char ch = utf::nextChar(ptr);
		if (utf::isNewLine(ch) && (!allowMultipleLines || charactersLeft < 0)) {
			break;
		}

		if (charactersTillEmphasizeStart == 0) {
			result += "\x1B[0;31m"; // Red color
		}

		if (charactersLeft == 0) {
			result += "\x1B[0m"; // Normal color
		}

		charactersLeft -= 1;
		charactersTillEmphasizeStart -= 1;

		result += ch;
	}

	if (charactersLeft >= 0) {
		result += "\x1B[0m"; // Normal color
	}

	return result;
}

uint64_t utils::TextPosition::getIndexInText(TextPosition position, utf::StringView text) {
	if (text.empty()) {
		return utf::String::npos;
	}

	const char* end = text.data() + text.size();
	const char* ptr = text.data();
	utf::Char ch = utf::nextChar(ptr);

	// First we look for the required line.
	while (position.line != 0 && ptr < end) {
		if (utf::skipToNextLine(ch, ptr, end)) {
			position.line -= 1;
		}
	}

	// The required line appeared to be out of text.
	if (position.line != 0) {
		return utf::String::npos;
	}

	// Now, we move on to the required character on the line.
	while (position.character != 0 && ptr < end) {
		if (utf::isNewLine(ch)) {
			return utf::String::npos; // Reached end of line, but not the required character.
		}

		ch = utf::nextChar(ptr);
		position.character -= 1;
	}

	return (ptr - text.data()) - utf::getCharSize(ch);
}

utf::String utils::TextPosition::toString() const {
	return std::format("{}:{}", line + 1, character + 1);
}
