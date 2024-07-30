// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	Expression.hpp by CoreJust
*	Created on 29.07.2024
*	Contains the Expression class that inherits the Node and is the base class for all expressions in AST.
*/

#pragma once
#include "Node.hpp"

namespace ast {
	class Expression : public Node {
	protected:
		Expression(NodeType type) noexcept;

	public:

	};
}
