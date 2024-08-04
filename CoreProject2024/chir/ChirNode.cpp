// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ChirNode.hpp"
#include <cassert>

thread_local uint64_t chir::Node::s_nodeIdCounter = 0;

chir::Node::Node(NodeType type, Node* parent) noexcept
	: m_parent(parent), m_nodeType(type), m_nodeId(s_nodeIdCounter++) { }

void chir::Node::setParent(utils::NoNull<Node> node, Node* parent) noexcept {
	node->m_parent = parent;
}

void chir::Node::setTextPosition(utils::TextPosition position) noexcept {
	m_position = position;
}
