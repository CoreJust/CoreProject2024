// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ModuleDeclarations.hpp"

ast::ModuleDeclarations::ModuleDeclarations(std::vector<utils::NoNull<ast::Declaration>> declarations) noexcept
	: Declaration(NodeKind::MODULE_DECLARATIONS), m_declarations(std::move(declarations)) {
	for (auto& declaration : m_declarations) {
		Node::setParent(declaration, this);
	}
}

const std::vector<utils::NoNull<ast::Declaration>>& ast::ModuleDeclarations::getDeclarations() const noexcept {
	return m_declarations;
}
