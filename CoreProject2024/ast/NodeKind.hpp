// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	NodeKind.hpp by CoreJust
*	Created on 29.07.2024
*	Contains the enumeration of possible AST node types.
*/

#pragma once
#include <cstdint>

namespace ast {
	enum class NodeKind : uint8_t {
		// Expression node types

		/*
		*	Literal value is a single number / character / boolean / string / etc. literal.
		*	e.g.: 0, 0x19, 'c', "text", true, '\u{19}'
		*/
		LITERAL_VALUE,

		/*
		*	A single identifier value. It can be either a variable or a function name.
		*/
		IDENTIFIER_VALUE,

		/*
		*	A function invocation operator. A specific kind of unary post operator:
		*	<expr>(<args>...), where expr type is some kind of callable type (e.g. a function or a type with defined invoke operator).
		*/
		INVOCATION_OPERATOR,

		/*
		*	Any general-kind unary pre operator:
		*	<operator><expr>
		*	e.g. +a, -<expr>
		*/
		UNARY_OPERATOR,

		/*
		*	Any general-kind binary operator:
		*	<left expr> <operator> <right expr>
		*	e.g. a + b, (x - y) / z
		*/
		BINARY_OPERATOR,

		/*
		*	Conditional binary operator that can be chained:
		*	<first expression> <op> <second expression> [<op> <nth expression>]...
		*	e.g. a == b == c, x != y, x < y > z
		*/
		COMPARATIVE_BINARY_OPERATOR,

		/*
		*	Return operator that returns the result from the function:
		*	return [<expr>] # <expr> is omitted in functions returning unit type.
		*	Also, it is implicitly used in expressions like fn...(...) = <expr>
		*/
		RETURN_OPERATOR,


		// Statement node types

		/*
		*	An adapter that allows usage of expressions in contexts where a statement is expected.
		*	e.g. <function>()
		*/
		EXPRESSION_STATEMENT,

		/*
		*	Scope statement is a scope of a number of statement and own visibility area for locals.
		*	{
		*		<statements>...
		*	}
		*/
		SCOPE_STATEMENT,


		// Subtype of statements - declarations (statements that can exist on the highest level, i.e. global objects)

		/*
		*	Creation of a variable:
		*	let/const <name>[:<type>] = <expr>
		*/
		VARIABLE_DECLARATION,

		/*
		*	Declaration of a functions:
		*	fn <name>(<args>...)[: <return-type>] <function-body>
		*/
		FUNCTION_DECLARATION,

		/*
		*	A list of global declarations of a single module (source file).
		*/
		MODULE_DECLARATIONS,

		NODE_TYPES_COUNT
	};

	// Checks if the given node type is an expression.
	constexpr bool isExpression(NodeKind type) noexcept {
		return static_cast<uint8_t>(type) < static_cast<uint8_t>(NodeKind::EXPRESSION_STATEMENT);
	}

	// Checks if the given node type is a statement.
	constexpr bool isStatement(NodeKind type) noexcept {
		return static_cast<uint8_t>(type) >= static_cast<uint8_t>(NodeKind::EXPRESSION_STATEMENT);
	}

	// Checks if the given node type is a declaration, i.e. statement of the highest level.
	constexpr bool isDeclaration(NodeKind type) noexcept {
		return static_cast<uint8_t>(type) >= static_cast<uint8_t>(NodeKind::VARIABLE_DECLARATION);
	}
}