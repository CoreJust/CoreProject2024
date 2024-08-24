// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "FunctionDeclaration.hpp"

ast::FunctionDeclaration::FunctionDeclaration(utf::StringView name, utils::NoNull<Type> returnType, std::vector<Argument> arguments, utils::NoNull<Statement> body) noexcept
	: Declaration(NodeKind::FUNCTION_DECLARATION), m_name(name), m_returnType(returnType), m_arguments(std::move(arguments)), m_body(body) {
	Node::setParent(body, this);
}

ast::FunctionDeclaration::FunctionDeclaration(utf::StringView name, utils::NoNull<Type> returnType, std::vector<Argument> arguments, utf::StringView nativeFunctionName) noexcept
	: Declaration(NodeKind::FUNCTION_DECLARATION), m_name(name), m_returnType(returnType), m_arguments(std::move(arguments)), m_body(nativeFunctionName) { }

utf::StringView ast::FunctionDeclaration::getName() const noexcept {
	return m_name;
}

utils::NoNull<ast::Type> ast::FunctionDeclaration::getReturnType() const noexcept {
	return m_returnType;
}

const std::vector<ast::FunctionDeclaration::Argument>& ast::FunctionDeclaration::getArguments() const noexcept {
	return m_arguments;
}

bool ast::FunctionDeclaration::isNative() const noexcept {
	return std::holds_alternative<utf::StringView>(m_body);
}

utils::NoNull<ast::Statement>& ast::FunctionDeclaration::getBodyAsStatement() noexcept {
	return std::get<utils::NoNull<Statement>>(m_body);
}

utf::StringView ast::FunctionDeclaration::getBodyAsNative() const noexcept {
	return std::get<utf::StringView>(m_body);
}
