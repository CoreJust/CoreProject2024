// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "LLVMGlobalsLoaderPass.hpp"
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Constants.h>
#include "utils/CollectionUtils.hpp"
#include "cir/CirModule.hpp"
#include "cir/value/constant/CirCommonFunction.hpp"
#include "cir/value/constant/CirGlobalVariable.hpp"
#include "cir/value/constant/CirConstanNumber.hpp"

cir_pass::LLVMGlobalsLoaderPass::LLVMGlobalsLoaderPass(llvm_utils::LLVMModule& llvmModule) noexcept
	: m_llvmModule(llvmModule) { }

void cir_pass::LLVMGlobalsLoaderPass::pass(utils::NoNull<cir::Module> module) {
	for (utils::NoNull<cir::GlobalValue> value : module->getGlobals()) {
		if (value->isFunction()) {
			loadFunction(value.as<cir::Function>());
		} else {
			loadGlobalVariable(value.as<cir::GlobalVariable>());
		}
	}
}

std::unordered_map<cir::ValueId, llvm::Value*> cir_pass::LLVMGlobalsLoaderPass::getLLVMGlobals() noexcept {
	return std::move(m_llvmGlobals);
}

void cir_pass::LLVMGlobalsLoaderPass::loadFunction(utils::NoNull<cir::Function> function) {
	std::vector<llvm::Type*> argumentTypes = utils::map<std::vector<llvm::Type*>>(
		function->getArguments(),
		[this](auto argument) -> auto {
			return argument->getType().makeLLVMType(m_llvmModule.getContext());
		}
	);

	llvm::FunctionType* functionType = llvm::FunctionType::get(function->getReturnType().makeLLVMType(m_llvmModule.getContext()), argumentTypes, false);
	llvm::Function::LinkageTypes linkage = 
		function->isNativeFunction() || function->getName() == "main" 
		? llvm::Function::ExternalLinkage : llvm::Function::InternalLinkage;

	llvm::Function* llvmFunction = llvm::Function::Create(
		functionType,
		linkage,
		function->getName(),
		m_llvmModule.getModule()
	);

	llvmFunction->setDSOLocal(true);

	int32_t index = 0;
	for (auto& arg : llvmFunction->args()) {
		arg.setName(function->getArguments()[index++]->getName());
	}

	m_llvmGlobals.try_emplace(function->getId(), llvmFunction);
}

void cir_pass::LLVMGlobalsLoaderPass::loadGlobalVariable(utils::NoNull<cir::GlobalVariable> variable) {
	const int64_t intValue = variable->getInitialValue()->getKind() == cir::ValueKind::CONSTANT_NUMBER
		? variable->getInitialValue().as<cir::ConstantNumber>()->getValue() : 0;

	llvm::Constant* initialValue = llvm::ConstantInt::get(m_llvmModule.getContext(), llvm::APInt(32, intValue, true));
	llvm::GlobalVariable* llvmVariable = new llvm::GlobalVariable(
		m_llvmModule.getModule(),
		variable->getType().makeLLVMType(m_llvmModule.getContext()),
		true,
		llvm::GlobalValue::InternalLinkage,
		initialValue,
		variable->getName()
	);

	m_llvmGlobals.try_emplace(variable->getId(), llvmVariable);
}
