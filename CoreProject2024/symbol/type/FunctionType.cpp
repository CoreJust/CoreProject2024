// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "FunctionType.hpp"
#include <format>
#include "utils/CollectionUtils.hpp"
#include "cir/type/CirFunctionType.hpp"
#include "TypeAllocator.hpp"

symbol::FunctionType::FunctionType(utils::NoNull<Type> returnType, std::vector<utils::NoNull<Type>> argumentTypes) noexcept 
	: Type(TypeKind::FUNCTION), m_returnType(returnType), m_argumentTypes(std::move(argumentTypes)) { }

utils::NoNull<symbol::FunctionType> symbol::FunctionType::make(utils::NoNull<Type> returnType, std::vector<utils::NoNull<Type>> argumentTypes) {
	return TypeAllocator::make<FunctionType>(returnType, std::move(argumentTypes));
}

utils::NoNull<cir::Type> symbol::FunctionType::makeCirType() const {
	return cir::FunctionType::make(
		m_returnType->makeCirType(), 
		utils::map<std::vector<utils::NoNull<cir::Type>>>(
			m_argumentTypes,
			[](auto argumentType) -> auto {
				return argumentType->makeCirType();
			}
		)
	);
}

bool symbol::FunctionType::equals(const Type& other) const noexcept {
	return other.getTypeKind() == TypeKind::FUNCTION;
}

utf::String symbol::FunctionType::toString() const noexcept {
	return std::format("fn({}): {}", utils::joinToString(m_argumentTypes), m_returnType->toString());
}

utf::String symbol::FunctionType::toMangleString() const noexcept {
	return std::format("fn({};{})", utils::joinToString(m_argumentTypes, ";"), m_returnType->toString());
}

const std::vector<utils::NoNull<symbol::Type>>& symbol::FunctionType::getArgumentTypes() const noexcept {
	return m_argumentTypes;
}

utils::NoNull<symbol::Type> symbol::FunctionType::getReturnType() const noexcept {
	return m_returnType;
}
