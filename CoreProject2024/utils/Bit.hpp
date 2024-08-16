// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	Bit.hpp by CoreJust
*	Created on 20.07.2024
*	Contains some auxiliary functions for bit manipulation.
*/

#pragma once
#include <bit>

#if defined(__INTEL_COMPILER) || defined(_MSC_VER)
#include <intrin.h>
#include <nmmintrin.h>
#include <immintrin.h>
#define ENABLE_INTRINSICS
#endif

namespace utils {
	// Returns the negative for the value as if it is a signed integer.
	constexpr auto negateUnsigned(std::unsigned_integral auto value) noexcept {
		return std::bit_cast<decltype(value)>(
			-std::bit_cast<std::make_signed_t<decltype(value)>>(value)
		);
	}

	// Parallel extract function.
	// Extracts from value only those bits that are set in mask
	// and remaps them to packed number.
	// For example (for u8): parallelExtract(value = 0b11010010, mask = 0b10101010) -> 0b1_0_0_1_.
	constexpr uint64_t parallelExtract(uint64_t value, uint64_t mask) noexcept {
#ifdef ENABLE_INTRINSICS
		if (std::is_constant_evaluated()) {
			uint64_t result = 0;
			uint64_t bit = 1;

			do {
				uint64_t lsb = mask & negateUnsigned(mask);
				mask &= ~lsb;
				result |= bool(value & lsb) * bit;
				bit <<= 1;
			} while (mask);

			return result;
		} else {
			return _pext_u64(value, mask);
		}
#else
		uint64_t result = 0;
		uint64_t bit = 1;

		do {
			uint64_t lsb = mask & negateUnsigned(mask);
			mask &= ~lsb;
			result |= bool(value & lsb) * bit;
			bit <<= 1;
		} while (mask);

		return result;
#endif
	}

	// 32-bit version of parallel extract.
	constexpr uint32_t parallelExtract(uint32_t value, uint32_t mask) noexcept {
#ifdef ENABLE_INTRINSICS
		if (std::is_constant_evaluated()) {
			uint32_t result = 0;
			uint32_t bit = 1;

			do {
				uint32_t lsb = mask & negateUnsigned(mask);
				mask &= ~lsb;
				result |= bool(value & lsb) * bit;
				bit <<= 1;
			} while (mask);

			return result;
		} else {
			return _pext_u32(value, mask);
		}
#else
		uint32_t result = 0;
		uint32_t bit = 1;

		do {
			uint32_t lsb = mask & negateUnsigned(mask);
			mask &= ~lsb;
			result |= bool(value & lsb) * bit;
			bit <<= 1;
		} while (mask);

		return result;
#endif
	}

	// Parallel deposit function.
	// Deposits the bits of the value to the given bits of the mask. (Opposite to the parallel extract)
	// For example (for u8): parallelExtract(value = 0b1_0_0_1_ = 0x1001, mask = 0b10101010) -> 0b11010010.
	constexpr uint64_t parallelDeposit(uint64_t value, uint64_t mask) noexcept {
#ifdef ENABLE_INTRINSICS
		if (std::is_constant_evaluated()) {
			uint64_t result = 0;
			for (uint64_t value_pos = 0, mask_pos = 0; mask_pos != 32; ++mask_pos) {
				if (mask >> mask_pos & 1) {
					result |= (value >> value_pos++ & 1) << mask_pos;
				}
			}

			return result;
		} else {
			return _pdep_u64(value, mask);
		}
#else
		uint64_t result = 0;
		for (uint64_t value_pos = 0, mask_pos = 0; mask_pos != 32; ++mask_pos) {
			if (mask >> mask_pos & 1) {
				result |= (value >> value_pos++ & 1) << mask_pos;
			}
		}

		return result;
#endif
	}

	// 32-bit version of parallel deposit.
	constexpr uint32_t parallelDeposit(uint32_t value, uint32_t mask) noexcept {
#ifdef ENABLE_INTRINSICS
		if (std::is_constant_evaluated()) {
			uint32_t result = 0;
			for (uint32_t value_pos = 0, mask_pos = 0; mask_pos != 32; ++mask_pos) {
				if (mask >> mask_pos & 1) {
					result |= (value >> value_pos++ & 1) << mask_pos;
				}
			}

			return result;
		} else {
			return _pdep_u32(value, mask);
		}
#else
		uint32_t result = 0;
		for (uint32_t value_pos = 0, mask_pos = 0; mask_pos != 32; ++mask_pos) {
			if (mask >> mask_pos & 1) {
				result |= (value >> value_pos++ & 1) << mask_pos;
			}
		}

		return result;
#endif
	}

	// Returns the index of the least significant bit.
	constexpr unsigned char leastSignificantBit(const uint64_t value) noexcept {
		if (std::is_constant_evaluated()) {
			return static_cast<unsigned char>(std::countr_zero(value));
		} else {
#ifdef ENABLE_INTRINSICS
			unsigned long result;
			_BitScanForward64(&result, value);

			return static_cast<unsigned char>(result);
#elif defined(__GNUC__)
			return static_cast<unsigned char>(__builtin_ctzll(value));
#else
			return static_cast<unsigned char>(std::countr_zero(value));
#endif
		}
	}

	// Returns the index of the most significant bit.
	constexpr unsigned char mostSignificantBit(const uint64_t value) noexcept {
		if (std::is_constant_evaluated()) {
			return static_cast<unsigned char>(63 - std::countl_zero(value));
		} else {
#ifdef ENABLE_INTRINSICS
			unsigned long result;
			_BitScanReverse64(&result, value);

			return static_cast<unsigned char>(result);
#elif defined(__GNUC__)
			return static_cast<unsigned char>(63 ^ __builtin_clzll(value));
#else
			return static_cast<unsigned char>(63 - std::countl_zero(value));
#endif
		}
	}

	// Returns the number of bits set to 1.
	constexpr unsigned char popCount(uint64_t value) noexcept {
		if (std::is_constant_evaluated()) {
			return static_cast<unsigned char>(std::popcount(value));
		} else {
#ifdef ENABLE_INTRINSICS
			return static_cast<unsigned char>(_mm_popcnt_u64(value));
#elif defined(__GNUC__)
			return static_cast<unsigned char>(__builtin_popcountll(value));
#else
			return static_cast<unsigned char>(std::popcount(value));
#endif
		}
	}
}