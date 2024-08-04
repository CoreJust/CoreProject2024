// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ChirModule.hpp"
#include "chir_visitor/ChirPrinter.hpp"

chir::Module::Module(std::vector<utils::NoNull<Declaration>> declarations) noexcept
	: m_declarations(std::move(declarations)) {

}

void chir::Module::print(utils::IndentPrinter& printer) const {
	chir_visitor::ChirPrinter chirPrinter(printer);

	for (auto declaration : m_declarations) {
		chirPrinter.print(declaration);
	}
}
