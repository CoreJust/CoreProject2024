// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ChirFunctionDeclaration.hpp"

chir::FunctionDeclaration::FunctionDeclaration(const symbol::FunctionSymbol& function, utils::NoNull<Statement> body) noexcept
	: Declaration(NodeKind::FUNCTION_DECLARATION), m_function(function), m_body(body) {
	Node::setParent(body, this);
}

chir::FunctionDeclaration::FunctionDeclaration(const symbol::FunctionSymbol& function, utf::String nativeFunctionName) noexcept
	: Declaration(NodeKind::FUNCTION_DECLARATION), m_function(function), m_body(std::move(nativeFunctionName)) {
}

chir::FunctionDeclaration::~FunctionDeclaration() {
	if (!isNative()) {
		getBodyAsStatement()->~Statement();
	}
}

const symbol::FunctionSymbol& chir::FunctionDeclaration::getFunction() const noexcept {
	return m_function;
}

bool chir::FunctionDeclaration::isNative() const noexcept {
	return std::holds_alternative<utf::String>(m_body);
}

utils::NoNull<chir::Statement>& chir::FunctionDeclaration::getBodyAsStatement() noexcept {
	return std::get<utils::NoNull<Statement>>(m_body);
}

utf::String chir::FunctionDeclaration::getBodyAsNative() const noexcept {
	return std::get<utf::String>(m_body);
}
