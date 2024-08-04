// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Node.hpp"
#include <cassert>

thread_local uint64_t ast::Node::s_nodeIdCounter = 0;

ast::Node::Node(NodeType type, Node* parent) noexcept
	: m_parent(parent), m_nodeType(type), m_nodeId(s_nodeIdCounter++) {

}

void ast::Node::setParent(utils::NoNull<Node> node, Node* parent) noexcept {
	node->m_parent = parent;
}

void ast::Node::setTextPosition(utils::TextPosition position) noexcept {
	m_position = position;
}
