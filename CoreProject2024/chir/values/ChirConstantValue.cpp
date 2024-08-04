// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ChirConstantValue.hpp"

chir::ConstantValue::ConstantValue(int64_t value) noexcept 
    : Value(NodeType::CONSTANT_VALUE, symbol::BasicType::I32), m_value(value) { }

int64_t chir::ConstantValue::getValue() const noexcept {
    return m_value;
}
