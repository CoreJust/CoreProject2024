// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirConstanNumber.hpp"

cir::ConstantNumber::ConstantNumber(Type type, int64_t value) noexcept
    : Constant(utf::String(reinterpret_cast<char*>(&value), sizeof(value)), std::move(type), ValueKind::CONSTANT_NUMBER) { }

int64_t cir::ConstantNumber::getValue() const noexcept {
    return *reinterpret_cast<const int64_t*>(m_name.data());
}
