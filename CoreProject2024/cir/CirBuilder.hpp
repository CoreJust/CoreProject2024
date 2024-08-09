// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirBuilder.hpp by CoreJust
*	Created on 05.08.2024
*	Contains CirBuilder class that is used to generate CIR code.
*/

#pragma once
#include "value/constant/CirBasicBlock.hpp"
#include "value/CirValueClassDeclarations.hpp"
#include "CirAllocator.hpp"

namespace cir {
	class CirBuilder final {
	private:
		CommonFunction* m_currentFunction = nullptr;
		BasicBlock* m_currentBasicBlock = nullptr;

	public:
		// Sets the given Function as the current one where the instructions will be added.
		// Resets the current basic block.
		void setFunction(utils::NoNull<CommonFunction> function);

		// Sets the given BasicBlock as the current one where the instructions will be added.
		void setBasicBlock(utils::NoNull<BasicBlock> basicBlock);

		// Creates and returns a new BasicBlock.
		// ! Does not change the current BasicBlock.
		utils::NoNull<BasicBlock> makeBasicBlock(utf::String name) noexcept;

		// Creates and returns a new BasicBlock, and makes it the current one.
		utils::NoNull<BasicBlock> makeAnsSetBasicBlock(utf::String name) noexcept;

		// Generates the FunctionArgument value.
		utils::NoNull<FunctionArgument> makeFunctionArgument(utf::String name, Type type);

		// Functions that create an Instruction. append it to the current BasicBlock, and return it.
		utils::NoNull<UnaryInstruction> makeNeg(utils::NoNull<Value> operand, utf::String name = "u");
		utils::NoNull<BinaryInstruction> makeAdd(utils::NoNull<Value> left, utils::NoNull<Value> right, utf::String name = "b");
		utils::NoNull<BinaryInstruction> makeSub(utils::NoNull<Value> left, utils::NoNull<Value> right, utf::String name = "b");
		utils::NoNull<BinaryInstruction> makeMul(utils::NoNull<Value> left, utils::NoNull<Value> right, utf::String name = "b");
		utils::NoNull<BinaryInstruction> makeDiv(utils::NoNull<Value> left, utils::NoNull<Value> right, utf::String name = "b");
		utils::NoNull<BinaryInstruction> makeRem(utils::NoNull<Value> left, utils::NoNull<Value> right, utf::String name = "b");
		utils::NoNull<InvocationInstruction> makeInvoke(utils::NoNull<Value> callee, std::vector<utils::NoNull<Value>> arguments, utf::String name = "i");
		utils::NoNull<UnitInvocationInstruction> makeUnitInvoke(utils::NoNull<Value> callee, std::vector<utils::NoNull<Value>> arguments);
		utils::NoNull<LocalVariable> makeLocal(utf::String name, Type type, utils::NoNull<Value> initialValue);
		utils::NoNull<RetInstruction> makeRet(utils::NoNull<Value> value);
		utils::NoNull<RetInstruction> makeRetUnit();

		CommonFunction* getCurrentFunction() const noexcept;
		BasicBlock* getCurrentBasicBlock() const noexcept;

	private:
		template<class T, class... Args> requires std::is_base_of_v<Instruction, T>
		utils::NoNull<T> makeInstruction(Args&&... args) {
			assert(m_currentBasicBlock != nullptr);

			utils::NoNull<T> result = CirAllocator::make<T, Args...>(std::forward<Args>(args)...);
			m_currentBasicBlock->addInstruction(result);

			return result;
		}
	};
}