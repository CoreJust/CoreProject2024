// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	NoNull.hpp by CoreJust
*	Created on 04.08.2024
*	Contains auxiliary NoNull class that ensures the underlying pointer is never nullptr.
*/

#pragma once
#include <concepts>
#include <source_location>
#include "error/InternalAssert.hpp"

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
		constexpr NoNull(T* ptr, const std::source_location& location = std::source_location::current()) noexcept : m_ptr(ptr) {
			error::internalAssert(ptr != nullptr, "Passed nullptr to NoNull class");
		}

		constexpr NoNull& operator=(const NoNull& other) noexcept {
			m_ptr = other.m_ptr;
			return *this;
		}

		constexpr NoNull& operator=(NoNull&& other) noexcept {
			m_ptr = other.m_ptr;
			return *this;
		}

		constexpr auto operator<=>(const T* other) const noexcept {
			return m_ptr <=> other;
		}
		
		constexpr bool operator==(const NoNull& other) const noexcept {
			return m_ptr == other.m_ptr;
		}

		constexpr T* operator->() const noexcept {
			return m_ptr;
		}

		constexpr T& operator*() const noexcept {
			return *m_ptr;
		}

		constexpr explicit operator T* () const noexcept {
			return m_ptr;
		}

		template<class Base> requires std::is_base_of_v<Base, T>
		constexpr operator NoNull<Base>() const noexcept {
			return NoNull<Base>(m_ptr);
		}

		template<class To>
		constexpr NoNull<To> as() const noexcept {
			return NoNull<To>(reinterpret_cast<To*>(m_ptr));
		}

		constexpr T* get() const noexcept {
			return m_ptr;
		}
	};
}