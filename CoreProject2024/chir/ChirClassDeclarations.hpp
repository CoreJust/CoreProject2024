// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ChirClassDeclarations.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the declarations of incomplete types for all the Node inheritors.
*	Used in other headers so as not to include the actual CHIR headers and lighten the compilation.
*/

#pragma once

namespace chir {
	class Node;
	class Value;
	class ConstantValue;
	class SymbolValue;
	class InvocationOperator;
	class UnaryOperator;
	class AsOperator;
	class BinaryOperator;
	class ReturnOperator;
	class Statement;
	class ValueStatement;
	class ScopeStatement;
	class IfElseStatement;
	class Declaration;
	class VariableStatement;
	class VariableDeclaration;
	class FunctionDeclaration;
}
