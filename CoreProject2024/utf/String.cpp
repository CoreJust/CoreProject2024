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
	const uint8_t* end = reinterpret_cast<const uint8_t*>(str.data()) + str.size();
	const uint8_t* ptr = reinterpret_cast<const uint8_t*>(str.data());

	while (ptr < end) {
		if (ptr + 8 <= end && (*reinterpret_cast<const uint64_t*>(ptr) & 0x8080808080808080) == 0) { // ASCII optimization
			ptr += 8;
			continue;
		}

		switch (*ptr & 0b11111000) {
			// 1-byte (ASCII) sequence
			case 0b00000000: [[fallthrough]];
			case 0b00001000: [[fallthrough]];
			case 0b00010000: [[fallthrough]];
			case 0b00011000: [[fallthrough]];
			case 0b00100000: [[fallthrough]];
			case 0b00101000: [[fallthrough]];
			case 0b00110000: [[fallthrough]];
			case 0b00111000: [[fallthrough]];
			case 0b01000000: [[fallthrough]];
			case 0b01001000: [[fallthrough]];
			case 0b01010000: [[fallthrough]];
			case 0b01011000: [[fallthrough]];
			case 0b01100000: [[fallthrough]];
			case 0b01101000: [[fallthrough]];
			case 0b01110000: [[fallthrough]];
			case 0b01111000: ptr += 1; break;
			// 2-byte sequence
			case 0b11000000: 
				if (*ptr < 0b11000100) return false; // Overlong character
				[[fallthrough]];
			case 0b11001000: [[fallthrough]];
			case 0b11010000: [[fallthrough]];
			case 0b11011000:
				if (ptr + 1 == end || (ptr[1] & 0xC0) != 0x80) {
					return false;
				} ptr += 2; break;
			// 3-byte sequence
			case 0b11100000:
				if (ptr + 2 >= end || (*reinterpret_cast<const uint16_t*>(ptr + 1) & 0xC0C0) != 0x8080
					|| (ptr[1] & 0b00100000) == 0) {
					return false;
				} ptr += 3; break;
			case 0b11101000:
				if (ptr + 2 >= end || (*reinterpret_cast<const uint16_t*>(ptr + 1) & 0xC0C0) != 0x8080) {
					return false;
				} if (*ptr == 0b11101101 && (ptr[1] & 0b00100000)) {
					return false; // Surrogate
				} ptr += 3; break;
			// 4-byte sequence
			case 0b11110000:
				if (ptr + 3 >= end || (*reinterpret_cast<const uint16_t*>(ptr + 1) & 0xC0C0) != 0x8080 || (ptr[3] & 0xC0) != 0x80) {
					return false;
				} if (*ptr == 0b11110000) {
					if ((ptr[1] & 0b00110000) == 0) return false; // Overlong
				} else if (*ptr == 0b11110100) {
					if (ptr[1] & 0b00100000) return false; // Too large
				} else if (*ptr > 0b11110100) {
					return false; // Too large
				} ptr += 4; break;
		default: return false; // Invalid sequence
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
