// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirModule.hpp"
#include "value/constant/CirGlobalVariable.hpp"
#include "value/constant/CirNativeFunction.hpp"
#include "value/constant/CirCommonFunction.hpp"
#include "CirAllocator.hpp"
#include "cir_pass/CirPrinterPass.hpp"

cir::Module::Module(utf::String name) noexcept 
    : m_name(std::move(name)), m_globalConstructor(CirAllocator::make<CommonFunction>("$GlobalInit", TypeKind::UNIT, std::vector<utils::NoNull<FunctionArgument>>(), this)) {
    m_globalConstructor->makeBasicBlock("entry");
}

void cir::Module::print(std::ostream& out) {
    utils::IndentPrinter indentPrinter(out);
    cir_pass::PrinterPass printerPass(indentPrinter);

    printerPass.pass(this);
}

utils::NoNull<cir::GlobalVariable> cir::Module::addGlobalVariable(utf::String name, Type type, utils::NoNull<Constant> initialValue) {
    return m_globals.emplace_back(CirAllocator::make<GlobalVariable>(std::move(name), std::move(type), initialValue, this)).as<GlobalVariable>();
}

utils::NoNull<cir::NativeFunction> cir::Module::addNativeFunction(utf::String name, Type returnType, std::vector<utils::NoNull<FunctionArgument>> arguments) {
    return m_globals.emplace_back(CirAllocator::make<NativeFunction>(std::move(name), std::move(returnType), std::move(arguments), this)).as<NativeFunction>();
}

utils::NoNull<cir::CommonFunction> cir::Module::addCommonFunction(utf::String name, Type returnType, std::vector<utils::NoNull<FunctionArgument>> arguments) {
    return m_globals.emplace_back(CirAllocator::make<CommonFunction>(std::move(name), std::move(returnType), std::move(arguments), this)).as<CommonFunction>();
}

utf::StringView cir::Module::getName() const noexcept {
    return m_name;
}

std::vector<utils::NoNull<cir::GlobalValue>>& cir::Module::getGlobals() noexcept {
    return m_globals;
}

utils::NoNull<cir::CommonFunction> cir::Module::getGlobalConstructor() const noexcept {
    return m_globalConstructor;
}
