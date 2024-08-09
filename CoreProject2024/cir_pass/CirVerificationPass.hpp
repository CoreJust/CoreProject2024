// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirVerificationPass.hpp by CoreJust
*	Created on 06.08.2024
*	Contains VerificationPass class that is derived from ModulePass
*	and that verifies CIR correctness.
*/

#pragma once
#include "CirPass.hpp"

namespace cir {
	class GlobalVariable;
}

namespace cir_pass {
	class VerificationPass final : public ModulePass {
	public:
		void pass(utils::NoNull<cir::Module> module) override;

	private:
		void verifyFunction(utils::NoNull<cir::Function> function);
		void verifyGlobalVariable(utils::NoNull<cir::GlobalVariable> variable);
		void verifyBasicBlock(utils::NoNull<cir::BasicBlock> basicBlock);

		void onVerificationFail(utf::String message);
	};
}