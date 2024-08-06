// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirCommonFunction.hpp"
#include "cir/CirAllocator.hpp"

cir::CommonFunction::CommonFunction(utf::String name, Type returnType, std::vector<utils::NoNull<FunctionArgument>> arguments, utils::NoNull<Module> parentModule) noexcept
    : Function(std::move(name), std::move(returnType), std::move(arguments), ValueKind::COMMON_FUNCTION, parentModule),
    m_instructionCounter(0) {

}

utils::NoNull<cir::BasicBlock> cir::CommonFunction::makeBasicBlock(utf::String name) noexcept {
    return m_basicBlocks.emplace_back(cir::CirAllocator::make<cir::BasicBlock>(*this, name + std::to_string(m_basicBlocks.size())));
}

std::vector<utils::NoNull<cir::BasicBlock>>& cir::CommonFunction::getBasicBlocks() noexcept {
    return m_basicBlocks;
}

uint64_t& cir::CommonFunction::getInstructionCounter() noexcept {
    return m_instructionCounter;
}
