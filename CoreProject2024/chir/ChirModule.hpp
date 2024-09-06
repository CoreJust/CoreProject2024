// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ChirModule.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the Module class that implement a single core module on Core HIR level.
*/

#pragma once
#include <vector>
#include "ChirDeclaration.hpp"
#include "utils/IndentPrinter.hpp"
#include "symbol/SymbolTable.hpp"

namespace chir {
	class Module final {
	private:
		std::vector<utils::NoNull<Declaration>> m_declarations;
		std::unique_ptr<symbol::SymbolTable> m_symbolTable;

	public:
		Module(std::vector<utils::NoNull<Declaration>> declarations, std::unique_ptr<symbol::SymbolTable> symbolTable) noexcept;
		~Module();

		void print(utils::IndentPrinter& printer);

		std::vector<utils::NoNull<Declaration>>& getDeclarations() noexcept;
	};
}