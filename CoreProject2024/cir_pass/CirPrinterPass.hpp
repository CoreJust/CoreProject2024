// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirPrinterPass.hpp by CoreJust
*	Created on 06.08.2024
*	Contains PrinterPass class that is derived from ModulePass
*	and allows printing CIR in a human-friendly form to an IndentPrinter.
*/

#pragma once
#include "utils/IndentPrinter.hpp"
#include "CirPass.hpp"

namespace cir {
	class GlobalVariable;
}

namespace cir_pass {
	class PrinterPass final : public ModulePass {
	private:
		utils::IndentPrinter& m_printer;

	public:
		PrinterPass(utils::IndentPrinter& printer) noexcept;

		void pass(utils::NoNull<cir::Module> module) override;

	private:
		void printFunction(utils::NoNull<cir::Function> function);
		void printGlobalVariable(utils::NoNull<cir::GlobalVariable> variable);
		void printBasicBlock(utils::NoNull<cir::BasicBlock> basicBlock);
	};
}