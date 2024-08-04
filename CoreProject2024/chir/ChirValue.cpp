// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ChirValue.hpp"
#include <cassert>

chir::Value::Value(NodeType type, symbol::Type valueType) noexcept : Node(type), m_type(std::move(valueType)) {
	assert(isValue());
}

symbol::Type& chir::Value::getValueType() noexcept {
	return m_type;
}
