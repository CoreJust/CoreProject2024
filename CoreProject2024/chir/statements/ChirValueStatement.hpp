// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ChirValueStatement.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the ValueStatement class that implements a CHIR node for an adapter for Value in Statement context (e.g. function call).
*/

#pragma once
#include "../ChirValue.hpp"
#include "../ChirStatement.hpp"

namespace chir {
	class ValueStatement final : public Statement {
	private:
		utils::NoNull<Value> m_value;

	public:
		ValueStatement(utils::NoNull<Value> value) noexcept;
		~ValueStatement() override;

		utils::NoNull<Value>& getValue() noexcept;
	};
}
