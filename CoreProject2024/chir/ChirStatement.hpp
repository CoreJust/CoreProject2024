// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ChirStatement.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the Statement class that inherits the Node and is the base class for all statements in CHIR.
*	It is equivalent to the Statement in AST.
*/

#pragma once
#include "ChirNode.hpp"

namespace chir {
	class Statement : public Node {
	protected:
		Statement(NodeKind type) noexcept;

	public:

	};
}