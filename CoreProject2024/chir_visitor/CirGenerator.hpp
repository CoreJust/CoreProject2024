// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirGenerator.hpp by CoreJust
*	Created on 06.08.2024
*	Contains CirGenerator class that is derived from ChirVisitor
*	and that compiles CHIR into CIR.
*/

#pragma once
#include <unordered_map>
#include "symbol/SymbolId.hpp"
#include "ChirVisitor.hpp"
#include "cir/CirBuilder.hpp"

namespace cir {
	class Module;
	class GlobalValue;
}

namespace chir_visitor {
	class CirGenerator final : public ChirVisitor </* Self = */CirGenerator, /* ValueResult = */utils::NoNull<cir::Value> , /* StatementResult = */void, /* DeclarationResult = */void > {
	private:
		using Parent = ChirVisitor<CirGenerator, utils::NoNull<cir::Value>, void, void>;

		friend class Parent;

	private:
		cir::Module& m_cirModule;
		cir::CirBuilder m_builder;
		std::unordered_map<symbol::SymbolId, utils::NoNull<cir::Value>> m_symbols;

	public:
		CirGenerator(cir::Module& cirModule, std::unordered_map<symbol::SymbolId, utils::NoNull<cir::Value>> globalsMap) noexcept;

		void visitRoot(chir::Module& module) override;

	private:
		utils::NoNull<cir::Value> visit(chir::ConstantValue& node) override;
		utils::NoNull<cir::Value> visit(chir::SymbolValue& node) override;
		utils::NoNull<cir::Value> visit(chir::InvocationOperator& node) override;
		utils::NoNull<cir::Value> visit(chir::UnaryOperator& node) override;
		utils::NoNull<cir::Value> visit(chir::BinaryOperator& node) override;
		utils::NoNull<cir::Value> visit(chir::ReturnOperator& node) override;
		void visit(chir::ValueStatement& node) override;
		void visit(chir::ScopeStatement& node) override;
		void visit(chir::IfElseStatement& node) override;
		void visit(chir::VariableStatement& node) override;
		void visit(chir::VariableDeclaration& node) override;
		void visit(chir::FunctionDeclaration& node) override;

	private:
		utils::NoNull<cir::Value> getSymbolValue(symbol::SymbolId symbolId) noexcept;
	};
}
