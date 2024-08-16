// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirPassManager.hpp by CoreJust
*	Created on 16.08.2024
*	Contains PassManager class that is derived from ModulePass
*	and that applies subsequent passes to each module given.
* 
*	The passes are applied in such an order:
* 
*	For each global value separately:
*	1) Basic block passes
*	2) Common function passes
*	3) Function passes
*	4) Global value passes
* 
*	After that:
*	5) Module passes
*/

#pragma once
#include <vector>
#include <memory>
#include "CirPass.hpp"

namespace cir_pass {
	class PassManager : public ModulePass {
	protected:
		std::vector<std::unique_ptr<ModulePass>> m_modulePasses;
		std::vector<std::unique_ptr<GlobalValuePass>> m_globalPasses;
		std::vector<std::unique_ptr<FunctionPass>> m_functionPasses;
		std::vector<std::unique_ptr<CommonFunctionPass>> m_commonFunctionPasses;
		std::vector<std::unique_ptr<BasicBlockPass>> m_basicBlockPasses;

	public:
		void pass(utils::NoNull<cir::Module> module) override;

		void registerPass(std::unique_ptr<ModulePass> modulePass);
		void registerPass(std::unique_ptr<GlobalValuePass> globalPass);
		void registerPass(std::unique_ptr<FunctionPass> functionPass);
		void registerPass(std::unique_ptr<CommonFunctionPass> commonFunctionPass);
		void registerPass(std::unique_ptr<BasicBlockPass> basicBlockPass);
	};
}