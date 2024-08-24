// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirConstant.hpp"
#include "error/InternalAssert.hpp"

cir::Constant::Constant(utf::String name, utils::NoNull<Type> type, ValueKind kind) noexcept
	: Value(std::move(name), std::move(type), kind) {
	error::internalAssert(cir::isConstant(kind));
}
