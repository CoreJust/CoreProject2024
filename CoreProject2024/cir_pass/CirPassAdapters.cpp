// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirPassAdapters.hpp"
#include "cir/CirModule.hpp"
#include "cir/value/constant/CirCommonFunction.hpp"

cir_pass::GlobalValuePassAdapter::GlobalValuePassAdapter(utils::NoNull<GlobalValuePass> pass) noexcept
	: m_pass(pass) { }

void cir_pass::GlobalValuePassAdapter::pass(utils::NoNull<cir::Module> module) {
	for (utils::NoNull<cir::GlobalValue> value : module->getGlobals()) {
		m_pass->pass(value);
	}
}

cir_pass::FunctionPassAdapter::FunctionPassAdapter(utils::NoNull<FunctionPass> pass) noexcept
	: m_pass(pass) { }

void cir_pass::FunctionPassAdapter::pass(utils::NoNull<cir::Module> module) {
	for (utils::NoNull<cir::GlobalValue> value : module->getGlobals()) {
		if (value->isFunction()) {
			m_pass->pass(value.as<cir::Function>());
		}
	}
}

cir_pass::CommonFunctionPassAdapter::CommonFunctionPassAdapter(utils::NoNull<CommonFunctionPass> pass) noexcept
	: m_pass(pass) { }

void cir_pass::CommonFunctionPassAdapter::pass(utils::NoNull<cir::Module> module) {
	for (utils::NoNull<cir::GlobalValue> value : module->getGlobals()) {
		if (value->isCommonFunction()) {
			m_pass->pass(value.as<cir::CommonFunction>());
		}
	}
}

cir_pass::BasicBlockPassAdapter::BasicBlockPassAdapter(utils::NoNull<BasicBlockPass> pass) noexcept
	: m_pass(pass) { }

void cir_pass::BasicBlockPassAdapter::pass(utils::NoNull<cir::CommonFunction> function) {
	for (utils::NoNull<cir::BasicBlock> basicBlock : function->getBasicBlocks()) {
		m_pass->pass(basicBlock);
	}
}

cir_pass::InstructionPassAdapter::InstructionPassAdapter(utils::NoNull<InstructionPass> pass) noexcept
	: m_pass(pass) { }

void cir_pass::InstructionPassAdapter::pass(utils::NoNull<cir::BasicBlock> value) {
	for (utils::NoNull<cir::Instruction> instruction : value->getInstructions()) {
		m_pass->pass(instruction);
	}
}
