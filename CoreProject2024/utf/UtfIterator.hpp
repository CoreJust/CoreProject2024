// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	UtfIterator.hpp by CoreJust
*	Created on 29.07.2024
*	Contains the UtfIterator and UtfCharacterRange classes that allow iteration over a Utf-8 string.
*/

#pragma once
#include "String.hpp"
#include "Utf.hpp"

namespace utf {
	/*
	*	Allows to iterate over Utf-8 characters of a Utf-8 string.
	*	Expect the string to be a valid Utf-8.
	*/
	class UtfIterator final {
	public:
		using value_type = utf::Char;
		using pointer = const utf::Char*;
		using reference = const utf::Char&;
		using difference_type = std::ptrdiff_t; // Not actually important
		using Iterator_category = std::forward_iterator_tag;

	private:
		const char* m_ptr;
		Char m_char;

	public:
		inline UtfIterator(const char* ptr) noexcept : m_ptr(ptr), m_char(extractChar(m_ptr)) { }

		constexpr UtfIterator(const UtfIterator&) noexcept = default;
		constexpr UtfIterator(UtfIterator&&) noexcept = default;

		constexpr UtfIterator& operator=(const UtfIterator& other) noexcept {
			m_ptr = other.m_ptr;
			return *this;
		}

		constexpr UtfIterator& operator=(UtfIterator&& other) noexcept {
			m_ptr = other.m_ptr;
			return *this;
		}

		constexpr bool operator==(const UtfIterator& other) const noexcept {
			return m_ptr == other.m_ptr;
		}

		constexpr auto operator<=>(const UtfIterator& other) const noexcept {
			return m_ptr <=> other.m_ptr;
		}

		constexpr reference operator*() const noexcept {
			return m_char;
		}

		constexpr pointer operator->() const noexcept {
			return &m_char;
		}

		// Prefix increment
		inline UtfIterator& operator++() {
			m_ptr += getCharSize(m_char);
			m_char = extractChar(m_ptr);
			return *this;
		}

		// Postfix increment
		inline UtfIterator operator++(int) {
			UtfIterator result = *this;
			m_ptr += getCharSize(m_char);
			m_char = extractChar(m_ptr);
			return result;
		}
	};

	// Used to be able to use range-based for with Utf-8 strings.
	class UtfCharacterRange final {
	private:
		const char* m_begin;
		const char* m_end;

	public:
		constexpr UtfCharacterRange(const char* _begin, const char* _end) noexcept : m_begin(_begin), m_end(_end) { }

		inline UtfIterator begin() noexcept {
			return UtfIterator(m_begin);
		}

		inline UtfIterator end() noexcept {
			return UtfIterator(m_end);
		}
	};

	/*
	*	Auxiliary function for iterating over a Utf-8 string.
	*	Usage:
	*		for (utf::Char ch : utf::charsOf(str)) ...
	*/
	UtfCharacterRange charsOf(utf::StringView str) noexcept {
		return UtfCharacterRange(str.data(), str.data() + str.size());
	}
}