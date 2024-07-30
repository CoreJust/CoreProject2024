// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	TokenizationUtils.hpp by CoreJust
*	Created on 25.07.2024
*	Contains some auxiliary utilities for Tokenizator.
*/

#pragma once
#include <array>
#include "utf/String.hpp"

namespace lexer {
	// Returns an array with true for indices equal to characters corresponding to identifier start.
	constexpr std::array<bool, 256> _MAKE_IDENTIFIER_START_CHARS() {
		std::array<bool, 256> result;

		for (int ch = 0; ch < 256; ch++) {
			result[ch] = (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_' || ch == '$';
		}

		return result;
	}

	// Returns an array with true for indices equal to characters corresponding to identifier middle.
	constexpr std::array<bool, 256> _MAKE_IDENTIFIER_MIDDLE_CHARS() {
		std::array<bool, 256> result;

		for (int ch = 0; ch < 256; ch++) {
			result[ch] = (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_' || ch == '$';
		}

		return result;
	}

	// For an array of given characters returns anothers array with trues set to indices corresponding to the chars of the given array.
	constexpr std::array<bool, 256> inverseArray(std::string arr) {
		std::array<bool, 256> result;
		result.fill(false);

		for (char ch : arr) {
			result[std::bit_cast<uint8_t>(ch)] = true;
		}

		return result;
	}

	constexpr std::array<bool, 256> _IDENTIFIER_START_CHARS = _MAKE_IDENTIFIER_START_CHARS();
	constexpr std::array<bool, 256> _IDENTIFIER_MIDDLE_CHARS = _MAKE_IDENTIFIER_MIDDLE_CHARS();
	constexpr std::array<bool, 256> _OPERATOR_CHARS = inverseArray("=+-/*%(){}:;,");

	// Returns true if ch is a valid character for identifier start.
	constexpr bool isIdentifierStart(utf::Char ch) noexcept {
		if (utf::getCharSize(ch) != 1) {
			return false;
		}

		return _IDENTIFIER_START_CHARS[utf::getFirstByteOf(ch)];
	}

	// Returns true if ch is a valid character for identifier but its first character.
	constexpr bool isIdentifierMiddle(utf::Char ch) noexcept {
		if (utf::getCharSize(ch) != 1) {
			return false;
		}

		return _IDENTIFIER_MIDDLE_CHARS[utf::getFirstByteOf(ch)];
	}

	// Returns true if ch is a valid character for an operator.
	constexpr bool isOperator(utf::Char ch) noexcept {
		if (utf::getCharSize(ch) != 1) {
			return false;
		}

		return _OPERATOR_CHARS[utf::getFirstByteOf(ch)];
	}
}