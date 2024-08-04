// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ChirStatement.hpp"
#include <cassert>

chir::Statement::Statement(NodeType type) noexcept : Node(type) {
	assert(isStatement());
}