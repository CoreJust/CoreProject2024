// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ChirReturnOperator.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the ReturnOperator class that implements a CHIR node for a return operator.
*/

#pragma once
#include "../ChirValue.hpp"

namespace chir {
	class ReturnOperator final : public Value {
	private:
		Value* m_value;

	public:
		ReturnOperator() noexcept;
		ReturnOperator(utils::NoNull<Value> value) noexcept;
		~ReturnOperator() override;

		Value*& getValue() noexcept;
	};
}
