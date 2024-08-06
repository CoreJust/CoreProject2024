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

utils::NoNull<cir::InvocationInstruction> cir::CirBuilder::makeInvoke(utils::NoNull<Value> callee, std::vector<utils::NoNull<Value>> arguments, utf::String name) {
    return makeInstruction<InvocationInstruction>(callee, std::move(arguments), std::move(name));
}

utils::NoNull<cir::UnitInvocationInstruction> cir::CirBuilder::makeUnitInvoke(utils::NoNull<Value> callee, std::vector<utils::NoNull<Value>> arguments) {
    return makeInstruction<UnitInvocationInstruction>(callee, std::move(arguments));
}

utils::NoNull<cir::LocalVariable> cir::CirBuilder::makeLocal(utf::String name, Type type, utils::NoNull<Value> initialValue) {
    return makeInstruction<LocalVariable>(std::move(name), std::move(type), initialValue);
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
