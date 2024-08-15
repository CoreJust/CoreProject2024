// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ChirUnaryOperator.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the UnaryOperator class that implements a CHIR node for an unary operator.
*/

#pragma once
#include "../ChirValue.hpp"

namespace chir {
	class UnaryOperator final : public Value {
	public:
		// The kind of unary operator used.
		enum UnaryOperatorType : uint8_t {
			PLUS = 0,
			MINUS,
			LOGIC_NOT,

			UNARY_OPERATOR_TYPES_COUNT
		};

	private:
		utils::NoNull<Value> m_value;
		UnaryOperatorType m_operatorType;

	public:
		UnaryOperator(UnaryOperatorType operatorType, utils::NoNull<Value> value, symbol::Type type) noexcept;

		utils::NoNull<Value>& getValue() noexcept;
		UnaryOperatorType getOperator() const noexcept;
	};
}
