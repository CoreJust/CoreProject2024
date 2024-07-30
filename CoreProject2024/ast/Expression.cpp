// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Expression.hpp"
#include <cassert>

ast::Expression::Expression(NodeType type) noexcept : Node(type) {
	assert(isExpression());
}
