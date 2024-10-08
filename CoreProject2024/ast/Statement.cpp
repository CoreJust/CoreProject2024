// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Statement.hpp"
#include <cassert>

ast::Statement::Statement(NodeKind type) noexcept : Node(type) {
	assert(isStatement());
}
