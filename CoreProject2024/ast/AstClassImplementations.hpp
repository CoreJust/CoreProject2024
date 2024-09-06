// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	AstClassImplementations.hpp by CoreJust
*	Created on 29.07.2024
*	Contains the includes of all the AST node headers for convenience.
*/

#pragma once
#include "expressions/LiteralValue.hpp"
#include "expressions/IdentifierValue.hpp"
#include "expressions/InvocationOperator.hpp"
#include "expressions/UnaryOperator.hpp"
#include "expressions/AsOperator.hpp"
#include "expressions/BinaryOperator.hpp"
#include "expressions/ComparativeBinaryOperator.hpp"
#include "expressions/ReturnOperator.hpp"
#include "statements/ExpressionStatement.hpp"
#include "statements/ScopeStatement.hpp"
#include "statements/IfElseStatement.hpp"
#include "declarations/VariableDeclaration.hpp"
#include "declarations/FunctionDeclaration.hpp"
#include "declarations/ModuleDeclarations.hpp"
