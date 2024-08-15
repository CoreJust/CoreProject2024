// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ChirConstantValue.hpp"

chir::ConstantValue::ConstantValue(symbol::Type type, int64_t value) noexcept
    : Value(NodeKind::CONSTANT_VALUE, std::move(type)), m_value(value) { }

int64_t chir::ConstantValue::getValue() const noexcept {
    return m_value;
}
