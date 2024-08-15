// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "SymbolLoader.hpp"
#include <cassert>
#include "symbol/SymbolAllocator.hpp"
#include "ast/declarations/VariableDeclaration.hpp"
#include "ast/declarations/FunctionDeclaration.hpp"
#include "ast/declarations/ModuleDeclarations.hpp"

ast_visitor::SymbolLoader::SymbolLoader(symbol::SymbolTable& symbols) noexcept 
	: Parent(*this), m_symbols(symbols) { }

void ast_visitor::SymbolLoader::loadSymbols(ast::Declaration* node) {
	Parent::visit(node);
}

void ast_visitor::SymbolLoader::visit(ast::LiteralValue& node) { }
void ast_visitor::SymbolLoader::visit(ast::IdentifierValue& node) { }
void ast_visitor::SymbolLoader::visit(ast::InvocationOperator& node) { }
void ast_visitor::SymbolLoader::visit(ast::UnaryOperator& node) { }
void ast_visitor::SymbolLoader::visit(ast::BinaryOperator& node) { }
void ast_visitor::SymbolLoader::visit(ast::ComparativeBinaryOperator& node) { }
void ast_visitor::SymbolLoader::visit(ast::ReturnOperator& node) { }
void ast_visitor::SymbolLoader::visit(ast::ExpressionStatement& node) { }
void ast_visitor::SymbolLoader::visit(ast::ScopeStatement& node) { }

void ast_visitor::SymbolLoader::visit(ast::VariableDeclaration& node) {
	m_symbols.addVariable(symbol::SymbolPath { }, utf::String(node.getName()), node.getVariableType().makeSymbolType());
}

void ast_visitor::SymbolLoader::visit(ast::FunctionDeclaration& node) {
	std::vector<utils::NoNull<symbol::VariableSymbol>> arguments;
	arguments.reserve(node.getArguments().size());

	for (auto& argument : node.getArguments()) {
		arguments.emplace_back(symbol::SymbolAllocator::make<symbol::VariableSymbol>(
			symbol::SymbolPath { }, 
			utf::String(argument.name), 
			argument.type.makeSymbolType()
		));
	}

	m_symbols.addFunction(symbol::SymbolPath { }, utf::String(node.getName()), node.getReturnType().makeSymbolType(), std::move(arguments));
}

void ast_visitor::SymbolLoader::visit(ast::ModuleDeclarations& node) {
	for (auto declaration : node.getDeclarations()) {
		Parent::visit(declaration);
	}
}
