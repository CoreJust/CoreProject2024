// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	Declaration.hpp by CoreJust
*	Created on 30.07.2024
*	Contains the Declaration class that inherits the Statement and is the base class for all declaration statements in AST.
*/

#pragma once
#include "Statement.hpp"

namespace ast {
	class Declaration : public Statement {
	protected:
		Declaration(NodeKind type) noexcept;

	public:

	};
}
