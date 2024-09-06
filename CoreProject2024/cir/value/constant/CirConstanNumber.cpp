// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirConstanNumber.hpp"

// Low-level memory manipulation so as not to cause destructor invocation for value while putting it into a string.
utf::String extractIntValue(utils::IntValue&& value) noexcept {
	utf::String result(reinterpret_cast<char*>(&value), sizeof(value));
	std::memset(&value, 0, sizeof(value));

	return result;
}

cir::ConstantNumber::ConstantNumber(utils::NoNull<Type> type, utils::IntValue value) noexcept
	: Constant(extractIntValue(std::move(value)), std::move(type), ValueKind::CONSTANT_NUMBER) { }

cir::ConstantNumber::~ConstantNumber() {
	// A way to call the default destructor.
	std::ignore = std::move(*reinterpret_cast<utils::IntValue*>(m_name.data()));
}

const utils::IntValue& cir::ConstantNumber::getValue() const noexcept {
	return *reinterpret_cast<const utils::IntValue*>(m_name.data());
}
