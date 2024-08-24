// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirTerminator.hpp"
#include "error/InternalAssert.hpp"

cir::Terminator::Terminator(utf::String name, utils::NoNull<Type> type, ValueKind kind) noexcept
	: Instruction(std::move(name), type, kind) {
	error::internalAssert(cir::isTerminator(kind));
}
