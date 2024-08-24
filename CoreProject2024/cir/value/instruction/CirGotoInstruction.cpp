// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirGotoInstruction.hpp"
#include <format>

cir::GotoInstruction::GotoInstruction(utils::NoNull<BasicBlock> basicBlockToGo) noexcept
    : Terminator("", Type::make(TypeKind::UNIT), ValueKind::GOTO_INSTRUCTION), m_basicBlockToGo(basicBlockToGo) {

}

utils::NoNull<cir::BasicBlock>& cir::GotoInstruction::getBasicBlockToGo() noexcept {
    return m_basicBlockToGo;
}

utf::String cir::GotoInstruction::toInstuctionString() const {
    return std::format("goto {}", m_basicBlockToGo->toString());
}
