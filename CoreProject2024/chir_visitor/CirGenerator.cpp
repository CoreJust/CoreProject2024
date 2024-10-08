// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirGenerator.hpp"
#include <format>
#include "utils/CollectionUtils.hpp"
#include "error/ErrorPrinter.hpp"
#include "chir/ChirModule.hpp"
#include "chir/ChirClassImplementations.hpp"
#include "cir/CirModule.hpp"
#include "cir/value/CirValueClassImplementations.hpp"

chir_visitor::CirGenerator::CirGenerator(cir::Module& cirModule, std::unordered_map<symbol::SymbolId, utils::NoNull<cir::Value>> globalsMap) noexcept 
	: Parent(*this), m_cirModule(cirModule), m_symbols(std::move(globalsMap)), m_builder() { }

void chir_visitor::CirGenerator::visitRoot(chir::Module& module) {
	for (utils::NoNull<chir::Declaration> declaration : module.getDeclarations()) {
		Parent::visit(declaration);
	}

	if (!m_cirModule.getGlobalConstructor()->getBasicBlocks().back()->hasTerminator()) {
		m_builder.setBasicBlock(m_cirModule.getGlobalConstructor()->getBasicBlocks().back());
		m_builder.makeRetUnit();
	}
}

utils::NoNull<cir::Value> chir_visitor::CirGenerator::visit(chir::ConstantValue& node) {
	return cir::CirAllocator::make<cir::ConstantNumber>(node.getValueType()->makeCirType(), node.getValue());
}

utils::NoNull<cir::Value> chir_visitor::CirGenerator::visit(chir::SymbolValue& node) {
	return getSymbolValue(node.getSymbol().getId());
}

utils::NoNull<cir::Value> chir_visitor::CirGenerator::visit(chir::InvocationOperator& node) {
	utils::NoNull<cir::Value> callee = Parent::visit(node.getCallee());
	std::vector<utils::NoNull<cir::Value>> arguments = utils::map<std::vector<utils::NoNull<cir::Value>>>(
		node.getArguments(),
		[this](auto argument) -> auto {
			return Parent::visit(argument);
		}
	);

	if (node.getValueType()->getTypeKind() == symbol::TypeKind::UNIT) {
		return m_builder.makeUnitInvoke(callee, std::move(arguments));
	} else {
		return m_builder.makeInvoke(callee, std::move(arguments));
	}
}

utils::NoNull<cir::Value> chir_visitor::CirGenerator::visit(chir::UnaryOperator& node) {
	utils::NoNull<cir::Value> operand = Parent::visit(node.getValue());

	switch (node.getOperator()) {
		case chir::UnaryOperator::MINUS:	 return m_builder.makeNeg(operand);
		case chir::UnaryOperator::BITWISE_NOT: return m_builder.makeBitwiseNot(operand);
		case chir::UnaryOperator::LOGIC_NOT: return m_builder.makeLogicNot(operand);
	default: return operand;
	}
}

utils::NoNull<cir::Value> chir_visitor::CirGenerator::visit(chir::AsOperator& node) {
	utils::NoNull<cir::Value> operand = Parent::visit(node.getValue());
	return m_builder.makeCast(operand, node.getValueType()->makeCirType());
}

utils::NoNull<cir::Value> chir_visitor::CirGenerator::visit(chir::BinaryOperator& node) {
	utils::NoNull<cir::Value> left = Parent::visit(node.getLeft());
	utils::NoNull<cir::Value> right = Parent::visit(node.getRight());

	switch (node.getOperator()) {
		case chir::BinaryOperator::PLUS:			return m_builder.makeAdd(left, right);
		case chir::BinaryOperator::MINUS:			return m_builder.makeSub(left, right);
		case chir::BinaryOperator::MULTIPLY:		return m_builder.makeMul(left, right);
		case chir::BinaryOperator::DIVIDE:			return m_builder.makeDiv(left, right);
		case chir::BinaryOperator::REMAINDER:		return m_builder.makeRem(left, right);
		case chir::BinaryOperator::BITWISE_AND:		return m_builder.makeBitwiseAnd(left, right);
		case chir::BinaryOperator::BITWISE_OR:		return m_builder.makeBitwiseOr(left, right);
		case chir::BinaryOperator::BITWISE_XOR:		return m_builder.makeBitwiseXor(left, right);
		case chir::BinaryOperator::BITWISE_LEFT_SHIFT: return m_builder.makeShl(left, right);
		case chir::BinaryOperator::BITWISE_RIGHT_SHIFT: return m_builder.makeShr(left, right);
		case chir::BinaryOperator::LOGICAL_AND:		return m_builder.makeLogicAnd(left, right);
		case chir::BinaryOperator::LOGICAL_OR:		return m_builder.makeLogicOr(left, right);
		case chir::BinaryOperator::EQUALS:			return m_builder.makeEq(left, right);
		case chir::BinaryOperator::NOT_EQUALS:		return m_builder.makeNeq(left, right);
		case chir::BinaryOperator::LESS_EQUALS:		return m_builder.makeLeq(left, right);
		case chir::BinaryOperator::GREATER_EQUALS:	return m_builder.makeGeq(left, right);
		case chir::BinaryOperator::LESS:			return m_builder.makeLt(left, right);
		case chir::BinaryOperator::GREATER:			return m_builder.makeGt(left, right);
	default: unreachable();
	}
}

utils::NoNull<cir::Value> chir_visitor::CirGenerator::visit(chir::ReturnOperator& node) {
	if (node.getValue() == nullptr) {
		return m_builder.makeRetUnit();
	} else {
		utils::NoNull<cir::Value> value = Parent::visit(node.getValue());
		return m_builder.makeRet(value);
	}
}

void chir_visitor::CirGenerator::visit(chir::ValueStatement& node) {
	Parent::visit(node.getValue());
}

void chir_visitor::CirGenerator::visit(chir::ScopeStatement& node) {
	for (utils::NoNull<chir::Statement> statement : node.getStatements()) {
		Parent::visit(statement);
	}
}

void chir_visitor::CirGenerator::visit(chir::IfElseStatement& node) {
	const std::vector<utils::NoNull<chir::Value>>& conditions = node.getConditions();
	const std::vector<utils::NoNull<chir::Statement>>& ifBodies = node.getIfBodies();

	utils::NoNull<cir::BasicBlock> mergeBlock = m_builder.makeBasicBlock("ifelsemerge");
	int64_t blocksCount = static_cast<int64_t>(ifBodies.size()) + node.hasElse();

	for (uint32_t i = 0; i < ifBodies.size(); ++i) {
		utils::NoNull<cir::Value> condition = Parent::visit(conditions[i]);
		utils::NoNull<cir::BasicBlock> successBranch = m_builder.makeBasicBlock("if");
		utils::NoNull<cir::BasicBlock> failureBranch = --blocksCount > 0 ?
			m_builder.makeBasicBlock("else") : mergeBlock;

		m_builder.makeBranch(condition, successBranch, failureBranch);
		m_builder.setBasicBlock(successBranch);
		Parent::visit(ifBodies[i]);
		m_builder.makeGoto(mergeBlock);

		m_builder.setBasicBlock(failureBranch);
	}

	if (node.hasElse()) {
		Parent::visit(node.getElseBody());
		m_builder.makeGoto(mergeBlock);
	}

	m_builder.setBasicBlock(mergeBlock);
}

void chir_visitor::CirGenerator::visit(chir::VariableStatement& node) {
	symbol::VariableSymbol& variable = node.getVariable();
	utils::NoNull<cir::Value> initialValue = Parent::visit(node.getInitialValue());
	m_symbols.try_emplace(variable.getId(), m_builder.makeLocal(variable.getName(), variable.getType()->makeCirType(), initialValue));
}

void chir_visitor::CirGenerator::visit(chir::VariableDeclaration& node) {
	const symbol::VariableSymbol& variable = node.getVariable();
	utils::NoNull<cir::GlobalVariable> variableValue = getSymbolValue(variable.getId()).as<cir::GlobalVariable>();

	m_builder.setBasicBlock(m_cirModule.getGlobalConstructor()->getBasicBlocks().back());
	utils::NoNull<cir::Value> initialValue = Parent::visit(node.getInitialValue());

	if (!initialValue->isConstant()) {
		error::ErrorPrinter::error({
			.code = error::ErrorCode::INTERNAL_ERROR,
			.name = "Internal error: Cannot use non-constant value for global variable initialization",
			.description = "Not supported yet."
		});

		internalAssert(false); // For debug mode.
		return;
	}

	variableValue->getInitialValue() = initialValue.as<cir::Constant>();
}

void chir_visitor::CirGenerator::visit(chir::FunctionDeclaration& node) {
	if (node.isNative()) {
		return;
	}

	const symbol::FunctionSymbol& function = node.getFunction();
	utils::NoNull<cir::CommonFunction> functionValue = getSymbolValue(function.getId()).as<cir::CommonFunction>();

	m_builder.setFunction(functionValue);
	utils::NoNull<cir::BasicBlock> entry = m_builder.makeAnsSetBasicBlock("entry");

	const std::vector<utils::NoNull<symbol::VariableSymbol>>& arguments = function.getArguments();
	for (uint32_t i = 0; i < arguments.size(); ++i) {
		m_symbols.try_emplace(arguments[i]->getId(), functionValue->getArguments()[i]);
	}

	Parent::visit(node.getBodyAsStatement());

	if (function.getReturnType()->getTypeKind() == symbol::TypeKind::UNIT && !m_builder.getCurrentBasicBlock()->hasTerminator()) {
		m_builder.makeRetUnit();
	}
}

utils::NoNull<cir::Value> chir_visitor::CirGenerator::getSymbolValue(symbol::SymbolId symbolId) noexcept {
	auto it = m_symbols.find(symbolId);
	if (it == m_symbols.end()) {
		error::ErrorPrinter::error({
			.code = error::ErrorCode::INTERNAL_ERROR,
			.name = "Internal error: Tried to get a symbol by unknown ID",
			.description = std::format( "Symbol map in CIR generator doesn't have symbol with ID {}.", symbolId),
		});

		internalAssert(false); // For debug mode.
	}

	return it->second;
}
