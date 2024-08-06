// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	ChirNode.hpp by CoreJust
*	Created on 03.08.2024
*	Contains the Node class that is base for all the CHIR nodes.
*/

#pragma once
#include "NodeKind.hpp"
#include "utils/NoNull.hpp"
#include "utils/TextPosition.hpp"

namespace chir {
	/*
	*	In CHIR we already have the information about symbols and types.
	*	CHIR structure is generally the same as AST, but with types inquired.
	*	Also, declarations in CHIR are not a subkind of statements.
	*/
	class Node {
	private:
		// Used to get unique ID's for each Node.
		static thread_local uint64_t s_nodeIdCounter;

	protected:
		utils::TextPosition m_position;
		Node* m_parent; // Pointer to the parent Node.
		const uint64_t m_nodeId; // Unique for each node within a single module.
		const NodeKind m_nodeKind; // For each final inheritor there is a separate NodeKind.

	protected:
		Node(NodeKind type, Node* parent = nullptr) noexcept;

		// To be used when the parent Node is constructed.
		static void setParent(utils::NoNull<Node> node, Node* parent) noexcept;

	public:
		Node() = delete;
		Node(const Node&) = default;
		Node(Node&&) = default;

		inline virtual ~Node() { }

		constexpr bool operator==(const Node& other) const noexcept {
			return m_nodeId == other.m_nodeId;
		}

		constexpr bool isValue() const noexcept {
			return chir::isExpression(m_nodeKind);
		}

		constexpr bool isStatement() const noexcept {
			return chir::isStatement(m_nodeKind);
		}

		constexpr bool isDeclaration() const noexcept {
			return chir::isDeclaration(m_nodeKind);
		}

		void setTextPosition(utils::TextPosition position) noexcept;


		// Getters

		constexpr Node* getParent() noexcept {
			return m_parent;
		}

		constexpr uint64_t getId() const noexcept {
			return m_nodeId;
		}

		constexpr NodeKind getKind() const noexcept {
			return m_nodeKind;
		}
	};
}
