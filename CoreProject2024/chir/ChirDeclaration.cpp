// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ChirDeclaration.hpp"
#include <cassert>

chir::Declaration::Declaration(NodeType type) noexcept : Node(type) {
	assert(isDeclaration());
}