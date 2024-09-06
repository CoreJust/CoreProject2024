// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ChirAsOperator.hpp by CoreJust
*	Created on 01.09.2024
*	Contains the AsOperator class that implements a CHIR node for a type convertion operator.
*/

#pragma once
#include "../ChirValue.hpp"

namespace chir {
	class AsOperator final : public Value {
	private:
		utils::NoNull<Value> m_value;

	public:
		AsOperator(utils::NoNull<Value> value, utils::NoNull<symbol::Type> type) noexcept;
		~AsOperator() override;

		utils::NoNull<Value>& getValue() noexcept;
	};
}
