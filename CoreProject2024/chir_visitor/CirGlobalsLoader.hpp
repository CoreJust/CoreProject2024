// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirGlobalsLoader.hpp by CoreJust
*	Created on 06.08.2024
*	Contains CirGlobalsLoader class that is derived from ChirVisitor
*	and that loads the symbols from CHIR to CIR Module.
*/

#pragma once
#include <unordered_map>
#include "ChirVisitor.hpp"
#include "symbol/Symbol.hpp"

namespace cir {
	class Module;
	class Value;
}

namespace chir_visitor {
	class CirGlobalsLoader final : public ChirVisitor</* Self = */CirGlobalsLoader, /* ValueResult = */void, /* StatementResult = */void, /* DeclarationResult = */void> {
	private:
		using Parent = ChirVisitor<CirGlobalsLoader, void, void, void>;

		friend class Parent;

	private:
		cir::Module& m_cirModule;
		std::unordered_map<symbol::SymbolId, utils::NoNull<cir::Value>> m_globalsMap;

	public:
		CirGlobalsLoader(cir::Module& cirModule) noexcept;

		void visitRoot(chir::Module& module) override;

		std::unordered_map<symbol::SymbolId, utils::NoNull<cir::Value>> getGlobalsMap();

	private:
		void visit(chir::ConstantValue& node) override;
		void visit(chir::SymbolValue& node) override;
		void visit(chir::InvocationOperator& node) override;
		void visit(chir::UnaryOperator& node) override;
		void visit(chir::BinaryOperator& node) override;
		void visit(chir::ReturnOperator& node) override;
		void visit(chir::ValueStatement& node) override;
		void visit(chir::ScopeStatement& node) override;
		void visit(chir::IfElseStatement& node) override;
		void visit(chir::VariableStatement& node) override;
		void visit(chir::VariableDeclaration& node) override;
		void visit(chir::FunctionDeclaration& node) override;
	};
}
