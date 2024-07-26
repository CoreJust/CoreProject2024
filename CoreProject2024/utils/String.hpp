// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	String.hpp by CoreJust
*	Created on 19.07.2024
*	Contains a number of auxiliary definitions for String type (which uses std::string, but can contain unicode characters).
*/

#pragma once
#include <string>
#include "Bit.hpp"
#include "Macro.hpp"

namespace utf {
	// Utf-8 string, string view and character respectively.
	using String = std::string;
	using StringView = std::string_view;
	using Char = char32_t;

	// Converts u8 chars to String.
	String asString(const char8_t* cstr);

	// Converts Utf-8 Char to String.
	String asString(Char ch);

	// Returns the first byte of the given Utf-8 character.
	constexpr char8_t getFirstByteOf(Char ch) noexcept {
		return static_cast<char8_t>((ch & 0xff000000) >> 24);
	}

	// Allows to get the size of a character by the first symbol (it can be either 1, 2, 3, or 4)
	// Returns 1 for bad characters.
	constexpr int getCharSize(char8_t firstByte) noexcept {
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
	constexpr int getCodePointSize(uint32_t codePoint) noexcept {
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
		return bit::parallelDeposit(codePoint, UTF_INSERTION_BITS[size - 1]) | UTF_BITS[size - 1];
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
		return bit::parallelExtract(ch, UTF_INSERTION_BITS[size - 1]);
	}

	// Returns true if character is a new line Utf-8 character.
	// Assumes that character a valid Utf-8 character.
	// Does not consider CR+LF (these are considered to be 2 separate characters).
	constexpr bool isNewLine(Char ch) noexcept {
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
	constexpr bool isWhitespaceButNewLine(Char ch) noexcept {
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
	constexpr bool isAnyWhitespace(Char ch) noexcept {
		return isNewLine(ch) || isWhitespaceButNewLine(ch);
	}

	// Returns true if the character is a whitespace Utf-8 character, line-terminating ones included.
	// Assumes that character a valid Utf-8 character.
	constexpr bool isDigit(Char ch) noexcept {
		return ch >= encodeUtf('0') && ch <= encodeUtf('9');
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
