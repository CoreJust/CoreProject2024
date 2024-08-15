// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ChirSymbolValue.hpp"
#include <cassert>
#include "symbol/SymbolTable.hpp"

chir::SymbolValue::SymbolValue(utils::NoNull<symbol::Symbol> symbol) noexcept
    : Value(NodeKind::SYMBOL_VALUE, symbol->getKind() == symbol::SymbolKind::FUNCTION ? symbol::TypeKind::I32 : symbol.as<symbol::VariableSymbol>()->getType()), m_symbol(symbol) { }

symbol::SymbolKind chir::SymbolValue::getSymbolKind() const noexcept {
    return m_symbol->getKind();
}

const symbol::Symbol& chir::SymbolValue::getSymbol() const noexcept {
    return *m_symbol;
}

const symbol::FunctionSymbol& chir::SymbolValue::getFunction() const noexcept {
    assert(m_symbol->getKind() == symbol::FUNCTION);

    return *reinterpret_cast<symbol::FunctionSymbol*>(m_symbol.get());
}

const symbol::VariableSymbol& chir::SymbolValue::getVariable() const noexcept {
    assert(m_symbol->getKind() == symbol::VARIABLE);

    return *reinterpret_cast<symbol::VariableSymbol*>(m_symbol.get());
}
