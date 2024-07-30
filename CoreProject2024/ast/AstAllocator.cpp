// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "AstAllocator.hpp"
#include <cassert>

thread_local std::pmr::unsynchronized_pool_resource ast::AstAllocator::s_resource;
thread_local std::pmr::polymorphic_allocator<> ast::AstAllocator::s_allocator { &s_resource };

void ast::AstAllocator::clear() {
	s_resource.release();
}
