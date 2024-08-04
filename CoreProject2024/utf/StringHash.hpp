// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	StringHash.hpp by CoreJust
*	Created on 02.08.2024
*	Contains some compile-time functions that allow switch-case statements with strings.
*/

#pragma once
#include "utf/String.hpp"

namespace utils {
	// Adds the characters to the string hash.
	// E.g. the current hash is for the string "abc", then we can add character 'd'
	// to the hash and get the hash for the string "abcd".
	constexpr uint64_t addToHash(uint64_t currentHash, char ch) noexcept {
		return (currentHash << 8) | ch;
	}

	// Adds the characters to the string hash.
	// E.g. the current hash is for the string "abc", then we can add character 'd'
	// to the hash and get the hash for the string "abcd".
	// Utf-8 version.
	constexpr uint64_t addToHash(uint64_t currentHash, utf::Char ch) noexcept {
		const int charSize = utf::getCharSize(ch) * 8;
		return (currentHash << charSize) | (ch >> (32 - charSize));
	}

	// Removes the characters from the string hash.
	// E.g. the current hash is for the string "abc", then we can remove characters 'c'
	// from the hash and get the hash for the string "ab".
	constexpr uint64_t removeFromHash(uint64_t currentHash) noexcept {
		return currentHash >> 8;
	}

	// Removes the characters from the string hash.
	// E.g. the current hash is for the string "abc", then we can remove characters 'c'
	// from the hash and get the hash for the string "ab".
	// Utf-8 version.
	constexpr uint64_t removeFromHash(uint64_t currentHash, utf::Char ch) noexcept {
		const int charSize = utf::getCharSize(ch) * 8;
		return currentHash >> charSize;
	}

	// Only the first 8 bytes of the string can get into hash,
	// the others are ignored.
	constexpr uint64_t hashOf(utf::StringView str) noexcept {
		uint64_t result = 0;
		if (str.size() <= 8) {
			for (char ch : str) {
				result = addToHash(result, ch);
			}
		} else {
			for (size_t i = 0; i < 8; i++) {
				result = addToHash(result, str[i]);
			}
		}

		return result;
	}

	// Helper struct that builds a string hash up.
	// That hash can later be used in switch statement.
	struct StringHashBuilder final {
	private:
		uint64_t m_hash;
		size_t m_stringSize;

	public:
		constexpr StringHashBuilder() noexcept : m_hash(0), m_stringSize(0) { }
		constexpr StringHashBuilder(char ch) noexcept : m_hash(ch), m_stringSize(1) { }
		constexpr StringHashBuilder(utf::Char ch) noexcept : m_hash(0), m_stringSize(0) {
			consume(ch);
		}

		// Adds the character to the current hash.
		constexpr void consume(char ch) noexcept {
			if (m_stringSize++ >= 8) return;

			m_hash = addToHash(m_hash, ch);
		}

		// Adds the character to the current hash.
		constexpr void consume(utf::Char ch) noexcept {
			if (m_stringSize++ >= 8) return;

			m_hash = addToHash(m_hash, ch);
		}

		// Deletes the character from the current hash.
		constexpr void unconsume(char ch) noexcept {
			if (--m_stringSize >= 8) return;

			m_hash = removeFromHash(m_hash);
		}

		// Deletes the character from the current hash.
		constexpr void unconsume(utf::Char ch) noexcept {
			if (--m_stringSize >= 8) return;

			m_hash = removeFromHash(m_hash);
		}

		// Returns the current hash.
		constexpr uint64_t getHash() const noexcept {
			return m_hash;
		}
	};
}