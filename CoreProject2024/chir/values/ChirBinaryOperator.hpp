// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ChirBinaryOperator.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the BinaryOperator class that implements a CHIR node for a binary operator.
*/

#pragma once
#include "../ChirValue.hpp"

namespace chir {
	class BinaryOperator final : public Value {
	public:
		// The kind of unary operator used.
		enum BinaryOperatorType : unsigned char {
			PLUS = 0,
			MINUS,
			MULTIPLY,
			DIVIDE,
			REMAINDER,

			LOGICAL_AND,
			LOGICAL_OR,

			EQUALS,
			NOT_EQUALS,
			LESS_EQUALS,
			GREATER_EQUALS,
			LESS,
			GREATER,

			BINARY_OPERATOR_TYPES_COUNT
		};

	private:
		utils::NoNull<Value> m_left;
		utils::NoNull<Value> m_right;
		BinaryOperatorType m_operatorType;

	public:
		BinaryOperator(BinaryOperatorType operatorType, utils::NoNull<Value> left, utils::NoNull<Value> right, utils::NoNull<symbol::Type> type) noexcept;

		utils::NoNull<Value>& getLeft() noexcept;
		utils::NoNull<Value>& getRight() noexcept;
		BinaryOperatorType getOperator() const noexcept;
	};
}
