// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	NoNull.hpp by CoreJust
*	Created on 04.08.2024
*	Contains auxiliary NoNull class that ensures the underlying pointer is never nullptr.
*/

#pragma once
#include <cassert>
#include <concepts>
#include <source_location>
#include <format>
#include "error/ErrorPrinter.hpp"

namespace utils {
	/*
	*	NoNull ensures that the underlying pointer is never nullptr.
	*	It doesn't own the pointer and wouldn't destroy it nor manage its uniqueness.
	*/
	template<class T>
	class NoNull final {
	private:
		T* m_ptr;

	public:
		NoNull() = delete;
		constexpr NoNull(NoNull&& other) noexcept : m_ptr(other.m_ptr) { }
		constexpr NoNull(const NoNull& other) noexcept : m_ptr(other.m_ptr) { }
		constexpr NoNull(T* ptr, std::source_location location = std::source_location::current()) noexcept : m_ptr(ptr) {
			if (ptr == nullptr) {
				error::ErrorPrinter::error({
					.code = error::ErrorCode::INTERNAL_ERROR,
					.name = "Internal compiler error: Unexpected null pointer",
					.selectionStart = utils::TextPosition(),
					.selectionLength = 0,
					.description = std::format(
						"Passed nullptr to NoNull class, source: {} at {}:{}, function {}.", 
						location.file_name(), 
						location.line(), 
						location.column(), 
						location.function_name()
					),
					.explanation = "-"
				});

				assert(ptr != nullptr); // For debug mode.
			}
		}

		constexpr auto operator<=>(const T* other) const noexcept {
			return m_ptr <=> other;
		}

		constexpr T* operator->() const noexcept {
			return m_ptr;
		}

		constexpr T& operator*() const noexcept {
			return *m_ptr;
		}

		constexpr operator T* () const noexcept {
			return m_ptr;
		}

		template<class Base> requires std::is_base_of_v<Base, T>
		constexpr operator NoNull<Base>() const noexcept {
			return NoNull<Base>(m_ptr);
		}

		constexpr T* get() const noexcept {
			return m_ptr;
		}
	};
}