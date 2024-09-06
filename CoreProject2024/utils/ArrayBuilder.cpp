// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ArrayBuilder.hpp"
#include <memory_resource>

// From old PooledAllocator
/*


thread_local std::vector<uint32_t> g_pointersHis;
thread_local std::vector<utils::Destructor> g_destructors;

uint16_t utils::getDestructorIndex(Destructor destructor) {
	uint16_t destructorIndex = static_cast<uint16_t>(g_destructors.size());
	g_destructors.emplace_back(destructor);

	return destructorIndex;
}

void utils::PooledAllocatorImpl::release() {
	for (auto& node : m_nodes) {
		std::byte* restoredPtr = reinterpret_cast<std::byte*>(node.ptrLo | (static_cast<size_t>(g_pointersHis[node.ptrHiIndex]) << 32));
		g_destructors[node.destructorIndex](restoredPtr);
	}

	m_resource.release();
	m_nodes.clear();
}

std::byte* utils::PooledAllocatorImpl::allocate(size_t size, size_t align, uint16_t destructorIndex) {
	std::byte* result = reinterpret_cast<std::byte*>(m_resource.allocate(size, align));
	uint32_t ptrLo = static_cast<uint32_t>(reinterpret_cast<size_t>(result) & 0xffffffff);
	uint32_t ptrHi = static_cast<uint32_t>((reinterpret_cast<size_t>(result) >> 32) & 0xffffffff);
	auto it = std::find(g_pointersHis.begin(), g_pointersHis.end(), ptrHi);
	uint16_t ptrHiIndex = static_cast<uint16_t>(it - g_pointersHis.begin());

	if (it == g_pointersHis.end()) { // No such a high part of a pointer was stored.
		ptrHiIndex = static_cast<uint32_t>(g_pointersHis.size());
		g_pointersHis.emplace_back(ptrHi);
	}

	m_nodes.emplace_back(AllocationNode { .ptrLo = ptrLo, .ptrHiIndex = ptrHiIndex, .destructorIndex = destructorIndex });

	return result;
}

*/

struct ArrayAllocationNode final {
	void(*destructor)(std::byte*);
	std::byte* ptr;
	size_t objectSize;
	size_t objectCount;
};

thread_local std::pmr::unsynchronized_pool_resource g_arrayAllocator;
thread_local std::unordered_map<size_t, std::byte*> g_freeData;
thread_local std::vector<ArrayAllocationNode> g_nodes;

void utils::releaseArrayAllocator() {
	for (auto& node : g_nodes) {
		std::byte* end = node.ptr + node.objectCount * node.objectSize;
		for (std::byte* ptr = node.ptr; ptr < end; ptr += node.objectSize) {
			node.destructor(ptr);
		}
	}

	g_arrayAllocator.release();
}

std::byte* utils::reallocateData(std::byte* oldData, size_t oldSize, size_t newSize, size_t align) {
	auto it = g_freeData.find(newSize);
	if (it == g_freeData.end() || it->second == nullptr) { // Check if an actual allocation is required.
		it = g_freeData.try_emplace(newSize, reinterpret_cast<std::byte*>(g_arrayAllocator.allocate(newSize, align))).first;
	}

	std::byte* result = it->second;
	g_freeData[oldSize] = oldData;
	memcpy(oldData, result, oldSize);
	return result;
}

void utils::finalize(void(*destructor)(std::byte*), std::byte* ptr, size_t objectSize, size_t objectCount) {
	g_nodes.emplace_back(ArrayAllocationNode {
		.destructor = destructor,
		.ptr = ptr,
		.objectSize = objectSize,
		.objectCount = objectCount
	});
}
