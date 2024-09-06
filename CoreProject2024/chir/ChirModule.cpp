// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ChirModule.hpp"
#include "chir_visitor/ChirPrinter.hpp"

chir::Module::Module(std::vector<utils::NoNull<Declaration>> declarations, std::unique_ptr<symbol::SymbolTable> symbolTable) noexcept
	: m_declarations(std::move(declarations)), m_symbolTable(std::move(symbolTable)) {

}

chir::Module::~Module() {
	for (auto declaration : m_declarations) {
		declaration->~Declaration();
	}
}

void chir::Module::print(utils::IndentPrinter& printer) {
	chir_visitor::ChirPrinter chirPrinter(printer);
	chirPrinter.visitRoot(*this);
}

std::vector<utils::NoNull<chir::Declaration>>& chir::Module::getDeclarations() noexcept {
	return m_declarations;
}
