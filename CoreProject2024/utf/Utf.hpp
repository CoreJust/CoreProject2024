// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	Utf.hpp by CoreJust
*	Created on 15.08.2024
*	Contains a number of functions to work with utf-8 strings.
*/

#pragma once
#include "String.hpp"

namespace utf {
	// Converts u8 chars to String.
	String asString(const char8_t* cstr);

	// Converts Utf-8 Char to String.
	String asString(Char ch);

	// Returns the first byte of the given Utf-8 character.
	constexpr INLINE char8_t getFirstByteOf(Char ch) noexcept {
		return static_cast<char8_t>((ch & 0xff000000) >> 24);
	}

	// Allows to get the size of a character by the first symbol (it can be either 1, 2, 3, or 4)
	// Returns 1 for bad characters.
	constexpr INLINE int getCharSize(char8_t firstByte) noexcept {
		if ((firstByte & 0b11100000) == 0b11000000) {
			return 2;
		} else if ((firstByte & 0b11110000) == 0b11100000) {
			return 3;
		} else if ((firstByte & 0b11111000) == 0b11110000) {
			return 4;
		}

		return 1;
	}

	// An overloaded version for a whole Utf-8 character.
	constexpr int getCharSize(Char ch) noexcept {
		return getCharSize(getFirstByteOf(ch));
	}

	// Extracts a character from the pointer.
	// Assumes that the pointer points to a valid Utf-8 string.
	Char extractChar(const char* ch);

	// Extracts a character from the pointer and moves the pointer on to the next character.
	// Assumes that the pointer points to a valid Utf-8 string.
	Char nextChar(char const*& ptr);

	// Checks if the character is a valid Utf-8 code.
	constexpr bool isValidUtf(Char ch) noexcept {
		int size = getCharSize(ch);
		if (size == 1) {
			return (ch & 0x80000000) == 0; // Utf-8 codes of length one should always begin with 0 bit.
		}

		// All the bytes but the first one must begin with bits 10.
		do {
			ch <<= 8;
			if ((ch & 0xc0000000) != 0x80000000) {
				return false;
			}
		} while (--size > 1);

		return true;
	}

	// Checks if the String consists of valid Utf-8 characters.
	bool isValidUtf(StringView str);

	// Returns the number of bytes for the given code point of unicode.
	// Assumes that the code point is correct and right within the range of unicode values.
	constexpr INLINE int getCodePointSize(uint32_t codePoint) noexcept {
		if (codePoint <= 0x7f) {
			return 1;
		} else if (codePoint <= 0x7ff) {
			return 2;
		} else if (codePoint <= 0xffff) {
			return 3;
		} else {
			return 4;
		}
	}

	// Encodes a unicode code point into Utf-8.
	constexpr Char encodeUtf(uint32_t codePoint) noexcept {
		// Preset bits of Utf-8 codes of correspondings sizes.
		const Char UTF_BITS[4] = {
			0b00000000'00000000'00000000'00000000,
			0b11000000'10000000'00000000'00000000,
			0b11100000'10000000'10000000'00000000,
			0b11110000'10000000'10000000'10000000
		};

		// Bits where the codepoint bits are inserted.
		const Char UTF_INSERTION_BITS[4] = {
			0b01111111'00000000'00000000'00000000,
			0b00011111'00111111'00000000'00000000,
			0b00001111'00111111'00111111'00000000,
			0b00000111'00111111'00111111'00111111
		};

		int size = getCodePointSize(codePoint);
		return utils::parallelDeposit(codePoint, UTF_INSERTION_BITS[size - 1]) | UTF_BITS[size - 1];
	}

	// Decodes a Utf-8 into unicode codepoint.
	constexpr uint32_t decodeUtf(Char ch) noexcept {
		// Bits where the codepoint bits are inserted.
		const Char UTF_INSERTION_BITS[4] = {
			0b01111111'00000000'00000000'00000000,
			0b00011111'00111111'00000000'00000000,
			0b00001111'00111111'00111111'00000000,
			0b00000111'00111111'00111111'00111111
		};

		int size = getCharSize(ch);
		return utils::parallelExtract(ch, UTF_INSERTION_BITS[size - 1]);
	}

	// Returns true if character is a new line Utf-8 character.
	// Assumes that character a valid Utf-8 character.
	// Does not consider CR+LF (these are considered to be 2 separate characters).
	constexpr INLINE bool isNewLine(Char ch) noexcept {
		switch (ch) {
			case encodeUtf(0xA): [[fallthrough]]; // LF (line feed) or \n
			case encodeUtf(0xB): [[fallthrough]]; // VT (vertical tab) or \v
			case encodeUtf(0xC): [[fallthrough]]; // FF (form feed) or \f
			case encodeUtf(0xD): [[fallthrough]]; // CR (caret return) or \r
			case encodeUtf(0x85): [[fallthrough]]; // NEL (next line)
			case encodeUtf(0x2028): [[fallthrough]]; // LS (line separator)
			case encodeUtf(0x2029): return true; // PS (paragraph separator)
		default: return false;
		}
	}

	// Returns true if the character is a whitespace Utf-8 character but line-terminating ones.
	// Assumes that character a valid Utf-8 character.
	constexpr INLINE bool isWhitespaceButNewLine(Char ch) noexcept {
		switch (ch) {
			case encodeUtf(0x9): [[fallthrough]]; // tab or \t
			case encodeUtf(0x20): [[fallthrough]]; // Normal space
			case encodeUtf(0xA0): [[fallthrough]]; // NBSP (non-breaking space)
			case encodeUtf(0x1680): [[fallthrough]]; // Ogham space mark
			case encodeUtf(0x2000): [[fallthrough]]; // En quad
			case encodeUtf(0x2001): [[fallthrough]]; // Em quad
			case encodeUtf(0x2002): [[fallthrough]]; // En space
			case encodeUtf(0x2003): [[fallthrough]]; // Em space
			case encodeUtf(0x2004): [[fallthrough]]; // Three-per-em space
			case encodeUtf(0x2005): [[fallthrough]]; // Four-per-em space
			case encodeUtf(0x2006): [[fallthrough]]; // Six-per-em space
			case encodeUtf(0x2007): [[fallthrough]]; // Figure space
			case encodeUtf(0x2008): [[fallthrough]]; // Punctuation space
			case encodeUtf(0x2009): [[fallthrough]]; // Thin space
			case encodeUtf(0x200A): [[fallthrough]]; // Hair space
			case encodeUtf(0x202F): [[fallthrough]]; // Narrow no-break space
			case encodeUtf(0x205F): [[fallthrough]]; // Medium mathematical space
			case encodeUtf(0x3000): return true; // Ideographic space
		default: return false;
		}
	}

	// Returns true if the character is a digit.
	// Assumes that character a valid Utf-8 character.
	constexpr INLINE bool isAnyWhitespace(Char ch) noexcept {
		switch (ch) {
			case encodeUtf(0xA): [[fallthrough]]; // LF (line feed) or \n
			case encodeUtf(0xB): [[fallthrough]]; // VT (vertical tab) or \v
			case encodeUtf(0xC): [[fallthrough]]; // FF (form feed) or \f
			case encodeUtf(0xD): [[fallthrough]]; // CR (caret return) or \r
			case encodeUtf(0x85): [[fallthrough]]; // NEL (next line)
			case encodeUtf(0x2028): [[fallthrough]]; // LS (line separator)
			case encodeUtf(0x2029): [[fallthrough]]; // PS (paragraph separator)
			case encodeUtf(0x9): [[fallthrough]]; // tab or \t
			case encodeUtf(0x20): [[fallthrough]]; // Normal space
			case encodeUtf(0xA0): [[fallthrough]]; // NBSP (non-breaking space)
			case encodeUtf(0x1680): [[fallthrough]]; // Ogham space mark
			case encodeUtf(0x2000): [[fallthrough]]; // En quad
			case encodeUtf(0x2001): [[fallthrough]]; // Em quad
			case encodeUtf(0x2002): [[fallthrough]]; // En space
			case encodeUtf(0x2003): [[fallthrough]]; // Em space
			case encodeUtf(0x2004): [[fallthrough]]; // Three-per-em space
			case encodeUtf(0x2005): [[fallthrough]]; // Four-per-em space
			case encodeUtf(0x2006): [[fallthrough]]; // Six-per-em space
			case encodeUtf(0x2007): [[fallthrough]]; // Figure space
			case encodeUtf(0x2008): [[fallthrough]]; // Punctuation space
			case encodeUtf(0x2009): [[fallthrough]]; // Thin space
			case encodeUtf(0x200A): [[fallthrough]]; // Hair space
			case encodeUtf(0x202F): [[fallthrough]]; // Narrow no-break space
			case encodeUtf(0x205F): [[fallthrough]]; // Medium mathematical space
			case encodeUtf(0x3000): return true; // Ideographic space
		default: return false;
		}
	}

	// Returns true if the character is a whitespace Utf-8 character, line-terminating ones included.
	// Assumes that character a valid Utf-8 character.
	constexpr INLINE bool isDigit(Char ch) noexcept {
		return ch >= encodeUtf('0') && ch <= encodeUtf('9');
	}

	// Returns true if the character changes the flow of printed characters, e.g. new lines.
	// Assumes that character a valid Utf-8 character.
	constexpr INLINE bool isControlCharacter(Char ch) noexcept {
		switch (ch) {
			case encodeUtf(0x61C): [[fallthrough]]; // ARABIC LETTER MARK
			case encodeUtf(0x200E): [[fallthrough]]; // LEFT-TO-RIGHT MARK
			case encodeUtf(0x200F): [[fallthrough]]; // RIGHT-TO-LEFT MARK
			case encodeUtf(0x202A): [[fallthrough]]; // LEFT-TO-RIGHT EMBEDDING
			case encodeUtf(0x202B): [[fallthrough]]; // RIGHT-TO-LEFT EMBEDDING
			case encodeUtf(0x202C): [[fallthrough]]; // POP DIRECTIONAL FORMATTING
			case encodeUtf(0x202D): [[fallthrough]]; // LEFT-TO-RIGHT OVERRIDE
			case encodeUtf(0x202E): [[fallthrough]]; // RIGHT-TO-LEFt OVERRIDE
			case encodeUtf(0x2066): [[fallthrough]]; // LEFT-TO-RIGHT ISOLATE
			case encodeUtf(0x2067): [[fallthrough]]; // RIGHT-TO-LEFt ISOLATE
			case encodeUtf(0x2068): [[fallthrough]]; // FIRST STRONG ISOLATE
			case encodeUtf(0x2069): [[fallthrough]]; // POP DIRECTIONAL ISOLATE
			case encodeUtf(0xFFF9): [[fallthrough]]; // INTERLINEAR ANNOTATION ANCHOR
			case encodeUtf(0xFFFA): [[fallthrough]]; // INTERLINEAR ANNOTATION SEPARATOR
			case encodeUtf(0xFFFB): return true; // INTERLINEAR ANNOTATION TERMINATOR
		default: return isNewLine(ch) || ch < encodeUtf(' ') || (ch >= encodeUtf(0x80) && ch <= encodeUtf(0x9F));
		}
	}

	// Returns true if the pointer points at a new line Utf-8 character or a sequence of CR+LF,
	// skipping the character / CR+LF sequence and storing the next character to ch.
	// Assumes that the pointer points to a valid Utf-8 string.
	bool matchNewLineOrCrLf(Char& ch, char const*& ptr, const char* end);

	// Goes through the string until the end of line or end of string is reached.
	// If the end of string is reached, ptr points to end (or after it) and the function returns false, 
	// otherwise, ch contains the first character of the next line, and the function returns true.
	bool skipToNextLine(Char& ch, char const*& ptr, const char* end);
}

// Append a Utf-8 character to the string.
utf::String& operator+=(utf::String& str, utf::Char ch);
