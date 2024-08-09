// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirFunction.hpp"

cir::Function::Function(utf::String name, Type returnType, std::vector<utils::NoNull<FunctionArgument>> arguments, ValueKind kind, utils::NoNull<Module> parentModule) noexcept
	: GlobalValue(std::move(name), /* TMP */TypeKind::BASIC_BLOCK, kind, parentModule),
	m_returnType(std::move(returnType)),
	m_arguments(std::move(arguments)) {
	assert(cir::isFunction(kind));
}

const cir::Type& cir::Function::getReturnType() const noexcept {
	return m_returnType;
}

std::vector<utils::NoNull<cir::FunctionArgument>>& cir::Function::getArguments() noexcept {
	return m_arguments;
}
