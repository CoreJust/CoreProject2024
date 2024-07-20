/*
*	String.hpp by CoreJust
*	Created on 19.07.2024
*	Contains a number of auxiliary definitions for String type (which uses std::u8string).
*/

#include <string>
#include "Bit.hpp"

// Utf-8 string, string view and character respectively.
using String = std::u8string;
using StringView = std::u8string_view;
using Char = char32_t; // Note: for Char of sizes less than 4 bytes it is allowed to have whatever in the leftover bytes

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
constexpr bool isValidUtf(String str) {
	for (size_t i = 0; i < str.size();) {
		int size = getCharSize(str[i]);
		if (i + size >= str.size()) {
			return false; // The last character doesn't fit in the String.
		}

		Char ch = *(Char*)&str[i];
		if (!isValidUtf(ch)) {
			return false; // Invalid character code.
		}

		i += size;
	}

	return true;
}

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
