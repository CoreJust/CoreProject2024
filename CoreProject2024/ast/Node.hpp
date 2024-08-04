// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	Node.hpp by CoreJust
*	Created on 28.07.2024
*	Contains the base class Node for all the classes of AST.
*/

#pragma once
#include "NodeType.hpp"
#include "utils/NoNull.hpp"
#include "utils/TextPosition.hpp"

namespace ast {
	/*
	*	In AST we have no information about any symbols yet, and neither about types.
	*	It will be fetched later before generation Core High-level IR.
	*/
	class Node {
	private:
		// Used to get unique ID's for each Node.
		static thread_local uint64_t s_nodeIdCounter;

	protected:
		utils::TextPosition m_position;
		Node* m_parent; // Pointer to the parent Node.
		const uint64_t m_nodeId; // Unique for each node within a single module.
		const NodeType m_nodeType; // For each final inheritor there is a separate NodeType.

	protected:
		Node(NodeType type, Node* parent = nullptr) noexcept;

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

		void setTextPosition(utils::TextPosition position) noexcept;

		constexpr bool isExpression() const noexcept {
			return ast::isExpression(m_nodeType);
		}

		constexpr bool isStatement() const noexcept {
			return ast::isStatement(m_nodeType);
		}

		constexpr bool isDeclaration() const noexcept {
			return ast::isDeclaration(m_nodeType);
		}


		// Getters

		constexpr utils::TextPosition getPosition() const noexcept {
			return m_position;
		}

		constexpr Node* getParent() noexcept {
			return m_parent;
		}

		constexpr uint64_t getId() const noexcept {
			return m_nodeId;
		}

		constexpr NodeType getType() const noexcept {
			return m_nodeType;
		}
	};
}