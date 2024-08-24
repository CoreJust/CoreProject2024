// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "AstFunctionType.hpp"
#include <format>
#include "utils/CollectionUtils.hpp"
#include "AstTypeAllocator.hpp"
#include "symbol/type/FunctionType.hpp"

ast::FunctionType::FunctionType(utils::NoNull<Type> returnType, std::vector<utils::NoNull<Type>> argumentTypes) noexcept 
	: Type(TypeKind::FUNCTION), m_returnType(returnType), m_argumentTypes(std::move(argumentTypes)) { }

utils::NoNull<ast::FunctionType> ast::FunctionType::make(utils::NoNull<Type> returnType, std::vector<utils::NoNull<Type>> argumentTypes) {
	return AstTypeAllocator::make<FunctionType>(returnType, std::move(argumentTypes));
}

utils::NoNull<symbol::Type> ast::FunctionType::makeSymbolType() const {
	return symbol::FunctionType::make(
		m_returnType->makeSymbolType(), 
		utils::map<std::vector<utils::NoNull<symbol::Type>>>(
			m_argumentTypes,
			[](auto argumentType) -> auto {
				return argumentType->makeSymbolType();
			}
		)
	);
}

utf::String ast::FunctionType::toString() const noexcept {
	return std::format("fn({}): {}", utils::joinToString(m_argumentTypes), m_returnType->toString());
}

utils::NoNull<ast::Type> ast::FunctionType::getReturnType() const noexcept {
	return m_returnType;
}
