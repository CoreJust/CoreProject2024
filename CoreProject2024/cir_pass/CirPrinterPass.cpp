// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirPrinterPass.hpp"
#include <format>
#include "utils/CollectionUtils.hpp"
#include "cir/CirModule.hpp"
#include "cir/value/constant/CirCommonFunction.hpp"
#include "cir/value/constant/CirGlobalVariable.hpp"

cir_pass::PrinterPass::PrinterPass(utils::IndentPrinter& printer) noexcept 
	: m_printer(printer) { }

void cir_pass::PrinterPass::pass(utils::NoNull<cir::Module> module) {
	m_printer.stream() << "module " << module->getName() << "\n\n";

	m_printer.increaseIndent();
	for (utils::NoNull<cir::GlobalValue> value : module->getGlobals()) {
		if (value->isFunction()) {
			printFunction(value.as<cir::Function>());
		} else {
			printGlobalVariable(value.as<cir::GlobalVariable>());
		}
	}

	m_printer.decreaseIndent();
}

void cir_pass::PrinterPass::printFunction(utils::NoNull<cir::Function> function) {
	if (function->isNativeFunction()) {
		m_printer.stream() << "native ";
	}

	m_printer.stream() << std::format("fn {}({}): {}", function->getName(), utils::joinToString(function->getArguments()), function->getReturnType().toString());

	if (!function->isNativeFunction()) {
		m_printer.stream() << " {\n";
		for (utils::NoNull<cir::BasicBlock> basicBlock : function.as<cir::CommonFunction>()->getBasicBlocks()) {
			printBasicBlock(basicBlock);
		}

		m_printer.stream() << "}";
	}

	m_printer.stream() << "\n\n";
}

void cir_pass::PrinterPass::printGlobalVariable(utils::NoNull<cir::GlobalVariable> variable) {
	m_printer.stream() << std::format("let global {}: {} = {}\n\n", variable->getName(), variable->getType().toString(), variable->getInitialValue()->toString());
}

void cir_pass::PrinterPass::printBasicBlock(utils::NoNull<cir::BasicBlock> basicBlock) {
	m_printer.stream() << std::format("${}:\n", basicBlock->getName());
	for (utils::NoNull<cir::Instruction> instruction : basicBlock->getInstructions()) {
		m_printer.printIndent();
		m_printer.stream() << instruction->toInstuctionString() << '\n';
	}
}
