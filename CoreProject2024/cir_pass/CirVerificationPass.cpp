// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirVerificationPass.hpp"
#include <format>
#include "error/ErrorPrinter.hpp"
#include "cir/CirModule.hpp"
#include "cir/value/constant/CirCommonFunction.hpp"
#include "cir/value/constant/CirGlobalVariable.hpp"
#include "cir/value/instruction/CirRetInstuction.hpp"

void cir_pass::VerificationPass::pass(utils::NoNull<cir::Module> module) {
	for (utils::NoNull<cir::GlobalValue> value : module->getGlobals()) {
		if (value->isFunction()) {
			verifyFunction(value.as<cir::Function>());
		} else {
			verifyGlobalVariable(value.as<cir::GlobalVariable>());
		}
	}
}

void cir_pass::VerificationPass::verifyFunction(utils::NoNull<cir::Function> function) {
	if (!function->isNativeFunction()) {
		for (utils::NoNull<cir::BasicBlock> basicBlock : function.as<cir::CommonFunction>()->getBasicBlocks()) {
			verifyBasicBlock(basicBlock);
		}
	}
}

void cir_pass::VerificationPass::verifyGlobalVariable(utils::NoNull<cir::GlobalVariable> variable) {

}

void cir_pass::VerificationPass::verifyBasicBlock(utils::NoNull<cir::BasicBlock> basicBlock) {
	const std::list<utils::NoNull<cir::Instruction>>& instructions = basicBlock->getInstructions();
	if (instructions.empty()) {
		onVerificationFail(std::format("Empty basic block {} in function {}", basicBlock->getName(), basicBlock->getParentFunction().getName()));
	} else if (!instructions.back()->isTerminator()) {
		onVerificationFail(std::format("No terminator at the end of basic block {} in function {}", basicBlock->getName(), basicBlock->getParentFunction().getName()));
	} else if (instructions.back()->getKind() == cir::ValueKind::RET_INSTRUCTION) {
		cir::CommonFunction& parentFunction = basicBlock->getParentFunction();
		const utils::NoNull<cir::RetInstruction>& retInstruction = instructions.back().as<cir::RetInstruction>();
		if (!parentFunction.getReturnType()->equals(retInstruction->getReturnType())) {
			onVerificationFail(std::format(
				"Ret operand and function return type mismatch: {} and {}, in function {}", 
				parentFunction.getReturnType()->toString(),
				retInstruction->getReturnType()->toString(),
				parentFunction.getName()
			));
		}
	}
}

void cir_pass::VerificationPass::onVerificationFail(utf::String message) {
	error::ErrorPrinter::error({
		.code = error::ErrorCode::VERIFICATION_FAILED,
		.name = "CIR error: Failed CIR verification",
		.description = message
	});
}
