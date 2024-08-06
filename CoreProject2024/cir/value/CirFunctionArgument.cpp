// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirFunctionArgument.hpp"
#include <format>

cir::FunctionArgument::FunctionArgument(utf::String name, Type type) noexcept
	: Value(std::move(name), std::move(type), ValueKind::FUNCTION_ARGUMENT) { }

utf::String cir::FunctionArgument::toString() const {
	return std::format("{}: {}", m_name, m_type.toString());
}
