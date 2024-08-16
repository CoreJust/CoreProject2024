// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	FastFmt.hpp by CoreJust
*	Created on 09.08.2024
*	Contains a number of auxiliary functions for faster formatting.
*/

#pragma once
#include "String.hpp"
#include "utils/Bit.hpp"
#include "utils/Macro.hpp"

namespace utf {
	// Fast algorithm to get integral log2.
	constexpr int ilog2(uint32_t x) noexcept {
		return utils::mostSignificantBit(x | 1);
	}

	// Fast algorithm to get the number of decimal digits in 32 bit unsigned integer.
	INLINE int countDecimalDigits(uint32_t x) noexcept {
		static uint64_t table[] = {
			4294967296,  8589934582,  8589934582,  8589934582,  12884901788,
			12884901788, 12884901788, 17179868184, 17179868184, 17179868184,
			21474826480, 21474826480, 21474826480, 21474826480, 25769703776,
			25769703776, 25769703776, 30063771072, 30063771072, 30063771072,
			34349738368, 34349738368, 34349738368, 34349738368, 38554705664,
			38554705664, 38554705664, 41949672960, 41949672960, 41949672960,
			42949672960, 42949672960 };
		return (x + table[ilog2(x)]) >> 32;
	}

	// Fast conversion of 32-bit unsigned integer to string in a fixed buffer.
	INLINE utf::StringView fastToString(uint64_t x) noexcept {
		static thread_local char s_buffer[32];

		// An enumeration of numbers in 2 digits from 0 to 99.
		static const alignas(alignof(uint16_t)) char DIGITS[] = \
			"00010203040506070809"\
			"10111213141516171819"\
			"20212223242526272829"\
			"30313233343536373839"\
			"40414243444546474849"\
			"50515253545556575859"\
			"60616263646566676869"\
			"70717273747576777879"\
			"80818283848586878889"\
			"90919293949596979899";

		if (x == 0) { // A special case.
			return "0";
		}

		// Convert the number by 2 digits at once.
		char* rbuff = s_buffer + 32;
		while (x >= 10) {
			*reinterpret_cast<uint16_t*>(rbuff -= 2) = *reinterpret_cast<const uint16_t*>(DIGITS + (x % 100) * 2);
			x /= 100;
		}

		// Handle the leftover digit if there is.
		if (x) {
			*--rbuff = static_cast<char>(x) + '0';
		}

		return utf::StringView(rbuff, s_buffer + 32);
	}
}