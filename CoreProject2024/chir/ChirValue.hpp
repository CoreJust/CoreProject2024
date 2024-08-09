// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ChirValue.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the Value class that inherits the Node and is the base class for all values in CHIR.
*	It is equivalent to the Expression in AST.
*/

#pragma once
#include "ChirNode.hpp"
#include "symbol/types/Type.hpp"

namespace chir {
	class Value : public Node {
	protected:
		symbol::Type m_type;

	protected:
		Value(NodeKind type, symbol::Type valueType) noexcept;

	public:
		symbol::Type& getValueType() noexcept;
	};
}