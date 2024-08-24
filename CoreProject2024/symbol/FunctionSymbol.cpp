// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "FunctionSymbol.hpp"
#include <format>
#include "utils/CollectionUtils.hpp"
#include "type/FunctionType.hpp"

symbol::FunctionSymbol::FunctionSymbol(SymbolPath path, utf::String name, utils::NoNull<Type> returnType, std::vector<utils::NoNull<VariableSymbol>> arguments)
	: Symbol(std::move(path), std::move(name), SymbolKind::FUNCTION), m_returnType(std::move(returnType)), m_arguments(std::move(arguments)) {

}

utf::String symbol::FunctionSymbol::makeMangledName() const {
	if (m_name == "main") {
		return "main";
	}

	return std::format(
		"{}::{}${}${}", 
		utils::joinToString(m_path.internalPath.path, "."),
		m_name, 
		utils::joinToString(m_arguments, ":", "", "", [](auto argument) -> auto {
			return argument->getType()->toMangleString();
		}),
		m_returnType->toMangleString()
	);
}

utils::NoNull<symbol::Type> symbol::FunctionSymbol::getReturnType() const noexcept {
	return m_returnType;
}

const std::vector<utils::NoNull<symbol::VariableSymbol>>& symbol::FunctionSymbol::getArguments() const noexcept {
	return m_arguments;
}

utils::NoNull<symbol::Type> symbol::FunctionSymbol::getSymbolValueType() const {
	return FunctionType::make(
		m_returnType, 
		utils::map<std::vector<utils::NoNull<Type>>>(
			m_arguments,
			[](auto argument) -> auto {
				return argument->getType();
			}
		)
	);
}

utf::String symbol::FunctionSymbol::toString() const noexcept {
	return std::format("fn {}({}): {}", m_name, utils::joinToString(m_arguments, ", "), m_returnType->toString());
}
