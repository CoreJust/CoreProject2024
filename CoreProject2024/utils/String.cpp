// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "String.hpp"

using namespace utf;

String utf::asString(const char8_t* cstr) {
	return String(reinterpret_cast<const char*>(cstr));
}

String utf::asString(Char ch) {
	String result;
	result += ch;
	return result;
}

Char utf::extractChar(const char* ch) {
	switch (getCharSize(std::bit_cast<char8_t>(*ch))) {
		case 1: return Char(*ch) << 24;
		case 2: return Char(*reinterpret_cast<const uint16_t*>(ch)) << 16;
		case 3: return (Char(*reinterpret_cast<const uint16_t*>(ch)) << 16) | (Char(*(ch + 2)) << 8);
		case 4: return *reinterpret_cast<const Char*>(ch);
	default: unreachable();
	}
}

Char utf::nextChar(char const*& ptr) {
	Char result;
	switch (getCharSize(std::bit_cast<char8_t>(*ptr))) {
		case 1: result = Char(*ptr) << 24; ptr += 1; break;
		case 2: result = Char(*reinterpret_cast<const uint16_t*>(ptr)) << 16; ptr += 2; break;
		case 3: result = (Char(*reinterpret_cast<const uint16_t*>(ptr)) << 16) | (Char(*(ptr + 2)) << 8); ptr += 3; break;
		case 4: result = *reinterpret_cast<const Char*>(ptr); ptr += 4; break;
	default: unreachable();
	}

	return result;
}

bool utf::isValidUtf(StringView str) {
	const char* end = str.data() + str.size();
	for (const char* ptr = str.data(); ptr < end;) {
		int size = getCharSize(std::bit_cast<char8_t>(*ptr));
		if (ptr + size > end) {
			return false; // The last character doesn't fit in the String.
		}

		Char ch = nextChar(ptr);
		if (!isValidUtf(ch)) {
			return false; // Invalid character code.
		}
	}

	return true;
}

bool utf::matchNewLineOrCrLf(Char& ch, char const*& ptr, const char* end) {
	switch (ch) {
		case encodeUtf(0xD): // CR (caret return) or \r
			ch = nextChar(ptr);
			if (ptr < end && ch == encodeUtf(0xA)) { // If it is a CR+LF
				ch = nextChar(ptr);
			}

			return true;
		case encodeUtf(0xA): [[fallthrough]]; // LF (line feed) or \n
		case encodeUtf(0x85): [[fallthrough]]; // NEL (next line)
		case encodeUtf(0x2028): [[fallthrough]]; // LS (line separator)
		case encodeUtf(0x2029): ch = nextChar(ptr); return true; // PS (paragraph separator)
	default: return false;
	}
}

bool utf::skipToNextLine(Char& ch, char const*& ptr, const char* end) {
	while (ptr < end) {
		if (matchNewLineOrCrLf(ch, ptr, end)) {
			return true;
		} else {
			ch = nextChar(ptr);
		}
	}

	return false;
}

String& operator+=(String& str, Char ch) {
	switch (getCharSize(ch)) {
		case 4:
			str += std::bit_cast<char>(getFirstByteOf(ch));
			ch <<= 8;
			[[fallthrough]];
		case 3:
			str += std::bit_cast<char>(getFirstByteOf(ch));
			ch <<= 8;
			[[fallthrough]];
		case 2:
			str += std::bit_cast<char>(getFirstByteOf(ch));
			ch <<= 8;
			[[fallthrough]];
		case 1:
			str += std::bit_cast<char>(getFirstByteOf(ch));
			break;
	default: unreachable();
	}

	return str;
}
