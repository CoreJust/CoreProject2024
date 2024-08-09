// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "SymbolTable.hpp"
#include <cassert>
#include <ranges>
#include "utils/CollectionUtils.hpp"
#include "SymbolAllocator.hpp"

symbol::SymbolTable symbol::g_symbolTable;

symbol::SymbolTable::SymbolTable()
    : m_rootScope(std::make_unique<Scope>()) {
    m_currentScope = m_rootScope.get();
}

symbol::VariableSymbol& symbol::SymbolTable::addVariable(SymbolPath path, utf::String name, symbol::Type type) {
    utils::NoNull<VariableSymbol> variable = SymbolAllocator::make<VariableSymbol>(std::move(path), std::move(name), std::move(type));

    if (m_currentScope->isGlobal()) {
        std::vector<utils::NoNull<Symbol>>& symbolsWithSuchName = m_symbolsByName
            .try_emplace(variable->getName(), std::vector<utils::NoNull<Symbol>>())
            .first->second;

        symbolsWithSuchName.push_back(variable);
    }

    m_symbolsById.try_emplace(variable->getId(), variable);
    m_currentScope->symbols().emplace_back(variable);

    return *variable;
}

symbol::FunctionSymbol& symbol::SymbolTable::addFunction(SymbolPath path, utf::String name, symbol::Type returnType, std::vector<utils::NoNull<VariableSymbol>> arguments) {
    utils::NoNull<FunctionSymbol> function = SymbolAllocator::make<FunctionSymbol>(
        std::move(path), 
        std::move(name), 
        std::move(returnType), 
        std::move(arguments)
    );

    if (m_currentScope->isGlobal()) {
        std::vector<utils::NoNull<Symbol>>& symbolsWithSuchName = m_symbolsByName
            .try_emplace(function->getName(), std::vector<utils::NoNull<Symbol>>())
            .first->second;

        symbolsWithSuchName.push_back(function);
    }

    m_symbolsById.try_emplace(function->getId(), function);
    m_currentScope->symbols().emplace_back(function);
    
    return *function;
}

symbol::Scope& symbol::SymbolTable::pushScope(utf::String name, FunctionSymbol* function) {
    m_currentScope = &m_currentScope->addScope(std::move(name), function == nullptr ? m_currentScope->getFunction() : function);
    return *m_currentScope;
}

symbol::Scope& symbol::SymbolTable::pushFunctionScope(FunctionSymbol& function) {
    Scope& result = pushScope(function.getName(), &function);
    for (auto argument : function.getArguments()) {
        m_symbolsById.try_emplace(argument->getId(), argument.get());
        m_currentScope->symbols().emplace_back(argument.get());
    }

    return result;
}

symbol::Scope& symbol::SymbolTable::popScope() {
    assert(m_currentScope->getParent() != nullptr);

    m_currentScope = m_currentScope->getParent();
    return *m_currentScope;
}

symbol::Scope& symbol::SymbolTable::getCurrentScope() noexcept {
    return *m_currentScope;
}

std::vector<utils::NoNull<symbol::Symbol>> symbol::SymbolTable::getSymbols(utf::StringView name) const {
    std::vector<utils::NoNull<Symbol>> result;
    Scope* scope = m_currentScope;

    // Going up the scopes searching for the symbols.
    // We can return either the first variable to be found, or the functions up to the first variable.
    while (true) {
        for (auto sym : scope->symbols() | std::views::reverse) {
            if (sym->getName() != name) continue;

            // Found a symbol with the given name.
            if (sym->getKind() == symbol::VARIABLE) {
                if (result.empty()) {
                    result.push_back(sym);
                }

                return result;
            } else {
                result.push_back(sym);
            }
        }

        if (scope->isGlobal()) {
            break;
        }

        scope = scope->getParent();
    }

    return result;
}

const symbol::VariableSymbol* symbol::SymbolTable::getVariable(utf::StringView name) const {
    std::vector<utils::NoNull<Symbol>> symbols = getSymbols(name);
    if (symbols.empty() || symbols[0]->getKind() != symbol::VARIABLE || symbols.size() > 1) {
        return nullptr;
    }

    return reinterpret_cast<symbol::VariableSymbol*>(symbols[0].get());
}

const symbol::FunctionSymbol* symbol::SymbolTable::getFunction(utf::StringView name, const std::vector<symbol::Type>& argumentTypes) const {
    std::vector<utils::NoNull<Symbol>> symbols = getSymbols(name);
    if (symbols.empty() || symbols[0]->getKind() != symbol::FUNCTION) {
        return nullptr;
    }

    for (utils::NoNull<Symbol> symbol : symbols) {
        if (symbol->getKind() != symbol::FUNCTION) {
            continue; // Just in case
        } 

        const FunctionSymbol& function = *symbol.as<FunctionSymbol>();
        if (utils::areEqual(function.getArguments(), argumentTypes, [](utils::NoNull<VariableSymbol> var, const symbol::Type& type) -> bool { // Check the argument types for match.
            return var->getType() == type;
        })) {
            return &function;
        }
    }

    return nullptr;
}

symbol::Symbol* symbol::SymbolTable::getSymbolById(SymbolId id) const noexcept {
    auto it = m_symbolsById.find(id);
    return it != m_symbolsById.end() ? it->second.get() : nullptr;
}
