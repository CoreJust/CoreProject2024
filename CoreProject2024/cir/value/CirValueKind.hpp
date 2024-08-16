// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirValueKind.hpp by CoreJust
*	Created on 05.08.2024
*	Contains ValueKind enumeration that represents a kind of CIR value.
*/

#pragma once

namespace cir {
	enum class ValueKind : unsigned char {
		/// Constants

		// Literal constants
		CONSTANT_NUMBER = 0,

		// Global constant values
		GLOBAL_VARIABLE,
		COMMON_FUNCTION,
		NATIVE_FUNCTION,

		// Basic block
		BASIC_BLOCK,


		/// Instructions

		// Instructions with usable resulting value
		UNARY_INSTRUCTION, // Any instruction with a single operand and a return value.
		BINARY_INSTRUCTION, // Any instruction with 2 operands and a return value.
		INVOCATION_INSTRUCTION, // Non-unit function call.
		LOCAL_VARIABLE,

		// Instructions that produce no usable value
		UNIT_INVOCATION_INSTRUCTION, // Unit function call.
		RET_INSTRUCTION,
		GOTO_INSTRUCTION,
		BRANCH_INSTRUCTION,

		
		/// Function argument
		FUNCTION_ARGUMENT,


		VALUE_KINDS_COUNT
	};

	constexpr bool isConstant(ValueKind kind) noexcept {
		return kind < ValueKind::UNARY_INSTRUCTION;
	}

	constexpr bool isGlobalValue(ValueKind kind) noexcept {
		switch (kind) {
			case cir::ValueKind::GLOBAL_VARIABLE: [[fallthrough]];
			case cir::ValueKind::COMMON_FUNCTION: [[fallthrough]];
			case cir::ValueKind::NATIVE_FUNCTION: return true;
		default: return false;
		}
	}
	
	constexpr bool isFunction(ValueKind kind) noexcept {
		switch (kind) {
			case cir::ValueKind::COMMON_FUNCTION: [[fallthrough]];
			case cir::ValueKind::NATIVE_FUNCTION: return true;
		default: return false;
		}
	}

	constexpr bool isIntruction(ValueKind kind) noexcept {
		return kind >= ValueKind::UNARY_INSTRUCTION && kind < ValueKind::FUNCTION_ARGUMENT;
	}

	// A terminator is an instruction that causes the change of control flow.
	// They must be at the end of each BasicBlock and cannot be at their middle.
	constexpr bool isTerminator(ValueKind kind) noexcept {
		switch (kind) {
			case cir::ValueKind::RET_INSTRUCTION: [[fallthrough]];
			case cir::ValueKind::GOTO_INSTRUCTION: [[fallthrough]];
			case cir::ValueKind::BRANCH_INSTRUCTION: return true;
		default: return false;
		}
	}

	// Returns whether the Value can be used as an operand in the instructions.
	constexpr bool isUsable(ValueKind kind) noexcept {
		switch (kind) {
			case cir::ValueKind::UNIT_INVOCATION_INSTRUCTION: [[fallthrough]];
			case cir::ValueKind::GOTO_INSTRUCTION: [[fallthrough]];
			case cir::ValueKind::BRANCH_INSTRUCTION: [[fallthrough]];
			case cir::ValueKind::RET_INSTRUCTION: return false;
		default: return true;
		}
	}
}
