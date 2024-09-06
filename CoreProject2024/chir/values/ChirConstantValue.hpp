// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ChirConstantValue.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the ConstantValue class that implements a CHIR node for a constant (literal) value.
*/

#pragma once
#include "utils/IntValue.hpp"
#include "../ChirValue.hpp"

namespace chir {
	class ConstantValue final : public Value {
	private:
		utils::IntValue m_value;

	public:
		ConstantValue(utils::NoNull<symbol::Type> type, utils::IntValue value) noexcept;
		
		// Used for int literals the type of which is unspecified yet.
		void setIntLiteralType(utils::NoNull<symbol::Type> type) noexcept;

		// Used when the type cannot be inquired from context and the default type is used.
		void assignOwnLiteralType() noexcept;

		void setBoolLiteralType() noexcept;

		utils::IntValue getValue() const noexcept;
		utils::IntValue& getValueAsRef() noexcept;
	};
}