// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	SymbolTable.hpp by CoreJust
*	Created on 02.08.2024
*	Contains the SymbolTable class that contains the symbols of a single module.
*/

#pragma once
#include <memory>
#include <unordered_map>
#include <list>
#include "FunctionSymbol.hpp"
#include "Scope.hpp"

namespace symbol {
	/*
	*	First of all, the types must be loaded into the table.
	*	After that, the functions and variables are loaded with function types defined
	*	(So that further function choices might be done).
	* 
	*	The local symbols are stored to the symbolsById but not to the symbolsByName.
	*/

	class SymbolTable final {
	private:
		std::unordered_map<utf::StringView, std::vector<utils::NoNull<Symbol>>> m_symbolsByName;
		std::unordered_map<SymbolId, utils::NoNull<Symbol>> m_symbolsById;
		std::unique_ptr<Scope> m_rootScope;
		Scope* m_currentScope;

	public:
		SymbolTable();

		// Adds a new variable to the table.
		// It must be checked outside that there were no such variable previously.
		VariableSymbol& addVariable(SymbolPath path, utf::String name, utils::NoNull<Type> type);

		// Adds a new function to the table.
		// It must be checked outside that there were no such function previously.
		FunctionSymbol& addFunction(SymbolPath path, utf::String name, utils::NoNull<Type> returnType, std::vector<utils::NoNull<VariableSymbol>> arguments);

		// Adds a new local scope.
		// All the symbols will be added to this scope until it is popped or a new one is pushed.
		// Returns a reference to the new scope.
		Scope& pushScope(utf::String name, FunctionSymbol* function = nullptr);

		// Adds a scope for the given function symbol and returns it.
		// Adds function arguments as variables to the scope.
		Scope& pushFunctionScope(FunctionSymbol& function);

		// Removes the current scope and returns to its parent scope.
		// Returns a reference to the current scope after the previous was popped.
		Scope& popScope();

		// Returns the reference to the current scope.
		Scope& getCurrentScope() noexcept;

		// Returns the symbols with the given name.
		// If no such symbol exists, returns empty vector.
		std::vector<utils::NoNull<Symbol>> getSymbols(utf::StringView name) const;

		// Returns the variable with the given name.
		// If no such variable exists or the variable is not visible from the current scope, returns nullptr.
		const VariableSymbol* getVariable(utf::StringView name) const;

		// Returns the function with the given name and argument types.
		// If no such function exists or the function is not visible from the current scope, returns nullptr.
		const FunctionSymbol* getFunction(utf::StringView name, const std::vector<utils::NoNull<Type>>& argumentTypes) const;

		// Returns the symbol with the given id.
		// If no such symbol exists, returns nullptr.
		Symbol* getSymbolById(SymbolId id) const noexcept;
	};
}