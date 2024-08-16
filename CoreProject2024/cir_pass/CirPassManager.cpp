// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirPassManager.hpp"
#include "cir/CirModule.hpp"
#include "cir/value/constant/CirCommonFunction.hpp"

void cir_pass::PassManager::pass(utils::NoNull<cir::Module> module) {
	for (auto globalValue : module->getGlobals()) {
		if (globalValue->isCommonFunction()) {
			// First, apply passes to basic blocks.
			for (auto basicBlock : globalValue.as<cir::CommonFunction>()->getBasicBlocks()) {
				for (auto& basicBlockPass : m_basicBlockPasses) {
					basicBlockPass->pass(basicBlock);
				}
			}

			// Then to the common functions.
			for (auto& commonFunctionPass : m_commonFunctionPasses) {
				commonFunctionPass->pass(globalValue.as<cir::CommonFunction>());
			}
		}

		// Then to all the functions.
		if (globalValue->isFunction()) {
			for (auto& functionPass : m_functionPasses) {
				functionPass->pass(globalValue.as<cir::Function>());
			}
		}

		// And to the global values.
		for (auto& globalPass : m_globalPasses) {
			globalPass->pass(globalValue);
		}
	}

	// Finally, module passes.
	for (auto& modulePass : m_modulePasses) {
		modulePass->pass(module);
	}
}

void cir_pass::PassManager::registerPass(std::unique_ptr<ModulePass> modulePass) {
	m_modulePasses.emplace_back(std::move(modulePass));
}

void cir_pass::PassManager::registerPass(std::unique_ptr<GlobalValuePass> globalPass) {
	m_globalPasses.emplace_back(std::move(globalPass));
}

void cir_pass::PassManager::registerPass(std::unique_ptr<FunctionPass> functionPass) {
	m_functionPasses.emplace_back(std::move(functionPass));
}

void cir_pass::PassManager::registerPass(std::unique_ptr<CommonFunctionPass> commonFunctionPass) {
	m_commonFunctionPasses.emplace_back(std::move(commonFunctionPass));
}

void cir_pass::PassManager::registerPass(std::unique_ptr<BasicBlockPass> basicBlockPass) {
	m_basicBlockPasses.emplace_back(std::move(basicBlockPass));
}
