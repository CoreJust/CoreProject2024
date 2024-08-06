// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirGlobalsLoader.hpp"
#include "utils/CollectionUtils.hpp"
#include "chir/declarations/ChirVariableDeclaration.hpp"
#include "chir/declarations/ChirFunctionDeclaration.hpp"
#include "chir/ChirModule.hpp"
#include "cir/value/constant/CirConstanNumber.hpp"
#include "cir/value/constant/CirGlobalVariable.hpp"
#include "cir/value/constant/CirCommonFunction.hpp"
#include "cir/value/constant/CirNativeFunction.hpp"
#include "cir/value/CirFunctionArgument.hpp"
#include "cir/CirAllocator.hpp"
#include "cir/CirModule.hpp"

chir_visitor::CirGlobalsLoader::CirGlobalsLoader(cir::Module& cirModule) noexcept
	: Parent(*this), m_cirModule(cirModule) {
}

void chir_visitor::CirGlobalsLoader::visitRoot(chir::Module& module) {
	for (utils::NoNull<chir::Declaration> declaration : module.getDeclarations()) {
		switch (declaration->getKind()) {
			case chir::NodeKind::VARIABLE_DECLARATION: visit(*declaration.as<chir::VariableDeclaration>());
			case chir::NodeKind::FUNCTION_DECLARATION: visit(*declaration.as<chir::FunctionDeclaration>());
		default: unreachable();
		}
	}
}

std::unordered_map<symbol::SymbolId, utils::NoNull<cir::Value>> chir_visitor::CirGlobalsLoader::getGlobalsMap() {
	return std::move(m_globalsMap);
}

void chir_visitor::CirGlobalsLoader::visit(chir::ConstantValue& node) { }
void chir_visitor::CirGlobalsLoader::visit(chir::SymbolValue& node) { }
void chir_visitor::CirGlobalsLoader::visit(chir::InvocationOperator& node) { }
void chir_visitor::CirGlobalsLoader::visit(chir::UnaryOperator& node) { }
void chir_visitor::CirGlobalsLoader::visit(chir::BinaryOperator& node) { }
void chir_visitor::CirGlobalsLoader::visit(chir::ReturnOperator& node) { }
void chir_visitor::CirGlobalsLoader::visit(chir::ValueStatement& node) { }
void chir_visitor::CirGlobalsLoader::visit(chir::ScopeStatement& node) { }
void chir_visitor::CirGlobalsLoader::visit(chir::VariableStatement& node) { }

void chir_visitor::CirGlobalsLoader::visit(chir::VariableDeclaration& node) {
	symbol::VariableSymbol variableSymbol = node.getVariable();
	utils::NoNull<cir::GlobalVariable> variable = m_cirModule.addGlobalVariable(
		variableSymbol.getName(),
		variableSymbol.getType().makeCirType(),
		cir::CirAllocator::make<cir::ConstantNumber>(0)
	);

	m_globalsMap.try_emplace(variableSymbol.getId(), variable);
}

void chir_visitor::CirGlobalsLoader::visit(chir::FunctionDeclaration& node) {
	symbol::FunctionSymbol functionSymbol = node.getFunction();
	if (node.isNative()) {
		utils::NoNull<cir::NativeFunction> function = m_cirModule.addNativeFunction(
			functionSymbol.getName(),
			functionSymbol.getReturnType().makeCirType(),
			utils::map<std::vector<utils::NoNull<cir::FunctionArgument>>>(
				functionSymbol.getArguments(),
				[](auto argument) -> auto {
					return cir::CirAllocator::make<cir::FunctionArgument>(argument->getName(), argument->getType().makeCirType());
				}
			)
		);

		m_globalsMap.try_emplace(functionSymbol.getId(), function);
	} else {
		utils::NoNull<cir::CommonFunction> function = m_cirModule.addCommonFunction(
			functionSymbol.makeMangledName(),
			functionSymbol.getReturnType().makeCirType(),
			utils::map<std::vector<utils::NoNull<cir::FunctionArgument>>>(
				functionSymbol.getArguments(),
				[](auto argument) -> auto {
					return cir::CirAllocator::make<cir::FunctionArgument>(argument->getName(), argument->getType().makeCirType());
				}
			)
		);

		m_globalsMap.try_emplace(functionSymbol.getId(), function);
	}
}
