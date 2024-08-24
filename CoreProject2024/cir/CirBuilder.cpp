// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirBuilder.hpp"
#include "value/CirValueClassImplementations.hpp"

void cir::CirBuilder::setFunction(utils::NoNull<CommonFunction> function) {
	m_currentFunction = function.get();
	m_currentBasicBlock = nullptr;
}

void cir::CirBuilder::setBasicBlock(utils::NoNull<BasicBlock> basicBlock) {
	m_currentBasicBlock = basicBlock.get();
	m_currentFunction = &basicBlock->getParentFunction();
}

utils::NoNull<cir::BasicBlock> cir::CirBuilder::makeBasicBlock(utf::String name) noexcept {
	return m_currentFunction->makeBasicBlock(std::move(name));
}

utils::NoNull<cir::BasicBlock> cir::CirBuilder::makeAnsSetBasicBlock(utf::String name) noexcept {
	return m_currentBasicBlock = m_currentFunction->makeBasicBlock(std::move(name)).get();
}

utils::NoNull<cir::UnaryInstruction> cir::CirBuilder::makeNeg(utils::NoNull<Value> operand, utf::String name) {
	return makeInstruction<UnaryInstruction>(UnaryInstruction::NEG, operand, std::move(name));
}

utils::NoNull<cir::UnaryInstruction> cir::CirBuilder::makeLogicNot(utils::NoNull<Value> operand, utf::String name) {
	return makeInstruction<UnaryInstruction>(UnaryInstruction::LOGIC_NOT, operand, std::move(name));
}

utils::NoNull<cir::BinaryInstruction> cir::CirBuilder::makeAdd(utils::NoNull<Value> left, utils::NoNull<Value> right, utf::String name) {
	return makeInstruction<BinaryInstruction>(BinaryInstruction::ADD, left, right, std::move(name));
}

utils::NoNull<cir::BinaryInstruction> cir::CirBuilder::makeSub(utils::NoNull<Value> left, utils::NoNull<Value> right, utf::String name) {
	return makeInstruction<BinaryInstruction>(BinaryInstruction::SUB, left, right, std::move(name));
}

utils::NoNull<cir::BinaryInstruction> cir::CirBuilder::makeMul(utils::NoNull<Value> left, utils::NoNull<Value> right, utf::String name) {
	return makeInstruction<BinaryInstruction>(BinaryInstruction::MUL, left, right, std::move(name));
}

utils::NoNull<cir::BinaryInstruction> cir::CirBuilder::makeDiv(utils::NoNull<Value> left, utils::NoNull<Value> right, utf::String name) {
	return makeInstruction<BinaryInstruction>(BinaryInstruction::DIV, left, right, std::move(name));
}

utils::NoNull<cir::BinaryInstruction> cir::CirBuilder::makeRem(utils::NoNull<Value> left, utils::NoNull<Value> right, utf::String name) {
	return makeInstruction<BinaryInstruction>(BinaryInstruction::REM, left, right, std::move(name));
}

utils::NoNull<cir::BinaryInstruction> cir::CirBuilder::makeLogicAnd(utils::NoNull<Value> left, utils::NoNull<Value> right, utf::String name) {
	return makeInstruction<BinaryInstruction>(BinaryInstruction::LOGIC_AND, left, right, std::move(name));
}

utils::NoNull<cir::BinaryInstruction> cir::CirBuilder::makeLogicOr(utils::NoNull<Value> left, utils::NoNull<Value> right, utf::String name) {
	return makeInstruction<BinaryInstruction>(BinaryInstruction::LOGIC_OR, left, right, std::move(name));
}

utils::NoNull<cir::BinaryInstruction> cir::CirBuilder::makeEq(utils::NoNull<Value> left, utils::NoNull<Value> right, utf::String name) {
	return makeInstruction<BinaryInstruction>(BinaryInstruction::CMP_EQ, left, right, std::move(name));
}

utils::NoNull<cir::BinaryInstruction> cir::CirBuilder::makeNeq(utils::NoNull<Value> left, utils::NoNull<Value> right, utf::String name) {
	return makeInstruction<BinaryInstruction>(BinaryInstruction::CMP_NEQ, left, right, std::move(name));
}

utils::NoNull<cir::BinaryInstruction> cir::CirBuilder::makeLeq(utils::NoNull<Value> left, utils::NoNull<Value> right, utf::String name) {
	return makeInstruction<BinaryInstruction>(BinaryInstruction::CMP_LEQ, left, right, std::move(name));
}

utils::NoNull<cir::BinaryInstruction> cir::CirBuilder::makeGeq(utils::NoNull<Value> left, utils::NoNull<Value> right, utf::String name) {
	return makeInstruction<BinaryInstruction>(BinaryInstruction::CMP_GEQ, left, right, std::move(name));
}

utils::NoNull<cir::BinaryInstruction> cir::CirBuilder::makeLt(utils::NoNull<Value> left, utils::NoNull<Value> right, utf::String name) {
	return makeInstruction<BinaryInstruction>(BinaryInstruction::CMP_LT, left, right, std::move(name));
}

utils::NoNull<cir::BinaryInstruction> cir::CirBuilder::makeGt(utils::NoNull<Value> left, utils::NoNull<Value> right, utf::String name) {
	return makeInstruction<BinaryInstruction>(BinaryInstruction::CMP_GT, left, right, std::move(name));
}

utils::NoNull<cir::InvocationInstruction> cir::CirBuilder::makeInvoke(utils::NoNull<Value> callee, std::vector<utils::NoNull<Value>> arguments, utf::String name) {
	return makeInstruction<InvocationInstruction>(callee, std::move(arguments), std::move(name));
}

utils::NoNull<cir::UnitInvocationInstruction> cir::CirBuilder::makeUnitInvoke(utils::NoNull<Value> callee, std::vector<utils::NoNull<Value>> arguments) {
	return makeInstruction<UnitInvocationInstruction>(callee, std::move(arguments));
}

utils::NoNull<cir::LocalVariable> cir::CirBuilder::makeLocal(utf::String name, utils::NoNull<Type> type, utils::NoNull<Value> initialValue) {
	return makeInstruction<LocalVariable>(std::move(name), type, initialValue);
}

utils::NoNull<cir::GotoInstruction> cir::CirBuilder::makeGoto(utils::NoNull<BasicBlock> basicBlockToGo) {
	m_currentBasicBlock->addSuccessor(basicBlockToGo);

	return makeInstruction<GotoInstruction>(basicBlockToGo);
}

utils::NoNull<cir::BranchInstruction> cir::CirBuilder::makeBranch(utils::NoNull<Value> value, utils::NoNull<BasicBlock> successBranch, utils::NoNull<BasicBlock> failureBranch) {
	m_currentBasicBlock->addSuccessor(successBranch);
	m_currentBasicBlock->addSuccessor(failureBranch);

	return makeInstruction<BranchInstruction>(value, successBranch, failureBranch);
}

utils::NoNull<cir::RetInstruction> cir::CirBuilder::makeRet(utils::NoNull<Value> value) {
	return makeInstruction<RetInstruction>(value);
}

utils::NoNull<cir::RetInstruction> cir::CirBuilder::makeRetUnit() {
	return makeInstruction<RetInstruction>();
}

cir::CommonFunction* cir::CirBuilder::getCurrentFunction() const noexcept {
	return m_currentFunction;
}

cir::BasicBlock* cir::CirBuilder::getCurrentBasicBlock() const noexcept {
	return m_currentBasicBlock;
}
