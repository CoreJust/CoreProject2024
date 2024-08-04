// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	AstClassDeclarations.hpp by CoreJust
*	Created on 29.07.2024
*	Contains the declarations of incomplete types for all the Node inheritors.
*	Used in other headers so as not to include the actual AST headers and lighten the compilation.
*/

#pragma once

namespace ast {
	class Node;
	class Expression;
	class Statement;
	class Declaration;
	class LiteralValue;
	class IdentifierValue;
	class InvocationOperator;
	class UnaryOperator;
	class BinaryOperator;
	class ReturnOperator;
	class ExpressionStatement;
	class ScopeStatement;
	class VariableDeclaration;
	class FunctionDeclaration;
	class ModuleDeclarations;
}
