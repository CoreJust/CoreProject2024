// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirTerminator.hpp"
#include <cassert>

cir::Terminator::Terminator(utf::String name, Type type, ValueKind kind) noexcept 
	: Instruction(std::move(name), std::move(type), kind) {
	assert(cir::isTerminator(kind));
}
