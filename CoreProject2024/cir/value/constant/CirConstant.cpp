// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirConstant.hpp"
#include <cassert>

cir::Constant::Constant(utf::String name, Type type, ValueKind kind) noexcept
	: Value(std::move(name), std::move(type), kind) {
	assert(cir::isConstant(kind));
}
