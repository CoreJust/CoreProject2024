// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Declaration.hpp"
#include <cassert>

ast::Declaration::Declaration(NodeKind type) noexcept : Statement(type) {
	assert(isDeclaration());
}
