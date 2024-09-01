// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	NodeKind.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the enumeration of possible CHIR node types.
*/

#pragma once

namespace chir {
	enum class NodeKind : unsigned char {
		// Value node types

		/*
		*	Constant value is a single number / character / boolean / string / etc. literal or keyword such as true / false / null.
		*	e.g.: 0, 0x19, 'c', "text", true, '\u{19}', true, null
		*/
		CONSTANT_VALUE,

		/*
		*	A single variable or function value.
		*/
		SYMBOL_VALUE,


		// Operator node types

		/*
		*	Function call:
		*	e.g. <function>(<args>...)
		*/
		INVOCATION_OPERATOR,

		/*
		*	Any unary operator:
		*	e.g. +a, -<expr>
		*/
		UNARY_OPERATOR,

		/*
		*	Type convertion operator:
		*	e.g. a as i8
		*/
		AS_OPERATOR,

		/*
		*	Any binary operator:
		*	e.g. a + b, (x - y) / z
		*/
		BINARY_OPERATOR,

		/*
		*	Return operator that returns the result from the function:
		*	return [<expr>] # <expr> is omitted in functions returning unit type.
		*/
		RETURN_OPERATOR,


		// Statement node types

		/*
		*	An adapter that allows usage of values and operators in contexts where a statement is expected.
		*	e.g. <function>()
		*/
		VALUE_STATEMENT,

		/*
		*	Scope statement is a scope of a number of statement and own visibility area for locals.
		*	{
		*		<statements>...
		*	}
		*/
		SCOPE_STATEMENT,

		/*
		*	Creation of a local variable:
		*	let/const <name>[:<type>] = <expr>
		*/
		VARIABLE_STATEMENT,

		/*
		*	Choosing one of a list of possible branches with a number of options:
		*	if (a) ...
		*	elif (b) ...
		*	else ...
		*/
		IF_ELSE_STATEMENT,


		// Declaration node types

		/*
		*	Creation of a global variable.
		*/
		VARIABLE_DECLARATION,

		/*
		*	Declaration of a functions:
		*	fn <name>(<args>...)[: <return-type>] <function-body>
		*/
		FUNCTION_DECLARATION,

		NODE_TYPES_COUNT
	};

	// Checks if the given node type is an expression.
	constexpr bool isExpression(NodeKind type) noexcept {
		return static_cast<unsigned char>(type) < static_cast<unsigned char>(NodeKind::VALUE_STATEMENT);
	}

	// Checks if the given node type is a statement.
	constexpr bool isStatement(NodeKind type) noexcept {
		return static_cast<unsigned char>(type) >= static_cast<unsigned char>(NodeKind::VALUE_STATEMENT) && static_cast<unsigned char>(type) < static_cast<unsigned char>(NodeKind::VARIABLE_DECLARATION);
	}

	// Checks if the given node type is a declaration, i.e. statement of the highest level.
	constexpr bool isDeclaration(NodeKind type) noexcept {
		return static_cast<unsigned char>(type) >= static_cast<unsigned char>(NodeKind::VARIABLE_DECLARATION);
	}
}
