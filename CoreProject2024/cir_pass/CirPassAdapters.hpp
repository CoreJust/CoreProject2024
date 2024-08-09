// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirPassAdapters.hpp by CoreJust
*	Created on 06.08.2024
*	Contains several classes that allow to use diverse CIR passes
*	by providing them the required Values.
*/

#pragma once
#include "CirPass.hpp"

namespace cir_pass {
	class GlobalValuePassAdapter final : public ModulePass {
	private:
		utils::NoNull<GlobalValuePass> m_pass;

	public:
		GlobalValuePassAdapter(utils::NoNull<GlobalValuePass> pass) noexcept;

		void pass(utils::NoNull<cir::Module> module) override;
	};

	class FunctionPassAdapter final : public ModulePass {
	private:
		utils::NoNull<FunctionPass> m_pass;

	public:
		FunctionPassAdapter(utils::NoNull<FunctionPass> pass) noexcept;

		void pass(utils::NoNull<cir::Module> module) override;
	};

	class CommonFunctionPassAdapter final : public ModulePass {
	private:
		utils::NoNull<CommonFunctionPass> m_pass;

	public:
		CommonFunctionPassAdapter(utils::NoNull<CommonFunctionPass> pass) noexcept;

		void pass(utils::NoNull<cir::Module> module) override;
	};

	class BasicBlockPassAdapter final : public CommonFunctionPass {
	private:
		utils::NoNull<BasicBlockPass> m_pass;

	public:
		BasicBlockPassAdapter(utils::NoNull<BasicBlockPass> pass) noexcept;

		void pass(utils::NoNull<cir::CommonFunction> function) override;
	};

	class InstructionPassAdapter final : public BasicBlockPass {
	private:
		utils::NoNull<InstructionPass> m_pass;

	public:
		InstructionPassAdapter(utils::NoNull<InstructionPass> pass) noexcept;

		void pass(utils::NoNull<cir::BasicBlock> value) override;
	};
}