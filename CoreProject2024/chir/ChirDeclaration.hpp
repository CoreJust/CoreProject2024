// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ChirDeclaration.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the Declaration class that inherits the Node and is the base class for all declarations in CHIR.
*	It is equivalent to the Declaration in AST, apart from the fact it is not a subkind of Statement.
*/

#pragma once
#include "ChirNode.hpp"

namespace chir {
	class Declaration : public Node {
	protected:
		Declaration(NodeKind type) noexcept;

	public:

	};
}