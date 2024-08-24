// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirFunction.hpp"
#include "utils/CollectionUtils.hpp"
#include "cir/type/CirFunctionType.hpp"

cir::Function::Function(utf::String name, utils::NoNull<Type> returnType, std::vector<utils::NoNull<FunctionArgument>> arguments, ValueKind kind, utils::NoNull<Module> parentModule) noexcept
	: GlobalValue(
		std::move(name), 
		FunctionType::make(returnType, utils::map<std::vector<utils::NoNull<Type>>>(
			arguments,
			[](auto argument) -> auto {
				return argument->getType();
			}
		)),
		kind, 
		parentModule
	),
	m_returnType(std::move(returnType)),
	m_arguments(std::move(arguments)) {
	error::internalAssert(cir::isFunction(kind));
}

utils::NoNull<cir::Type> cir::Function::getReturnType() const noexcept {
	return m_returnType;
}

std::vector<utils::NoNull<cir::FunctionArgument>>& cir::Function::getArguments() noexcept {
	return m_arguments;
}
