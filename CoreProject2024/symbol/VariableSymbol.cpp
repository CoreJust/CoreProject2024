// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "VariableSymbol.hpp"
#include <format>

symbol::VariableSymbol::VariableSymbol(SymbolPath path, utf::String name, symbol::Type type)
	: Symbol(std::move(path), std::move(name), SymbolKind::VARIABLE), m_type(std::move(type)) {

}

const symbol::Type& symbol::VariableSymbol::getType() const noexcept {
	return m_type;
}

utf::String symbol::VariableSymbol::toString() const noexcept {
	if (m_path.internalPath.path.empty()) {
		return std::format("{}: {}", m_name, m_type.toString());
	} else {
		return std::format("{}.{}: {}", m_path.internalPath.toString(), m_name, m_type.toString());
	}
}
