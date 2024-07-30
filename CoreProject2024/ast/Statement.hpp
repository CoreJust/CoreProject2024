// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	Statement.hpp by CoreJust
*	Created on 30.07.2024
*	Contains the Statement class that inherits the Node and is the base class for all statements and declarations in AST.
*/

#pragma once
#include "Node.hpp"

namespace ast {
	class Statement : public Node {
	protected:
		Statement(NodeType type) noexcept;

	public:

	};
}
