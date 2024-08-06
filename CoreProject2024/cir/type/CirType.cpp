// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirType.hpp"

cir::Type::Type(TypeKind kind) noexcept : m_typeKind(kind) {

}

bool cir::Type::operator==(const Type& other) const noexcept {
	return m_typeKind == other.getTypeKind();
}

cir::TypeKind cir::Type::getTypeKind() const noexcept {
	return m_typeKind;
}

uint32_t cir::Type::getTypeSize() const noexcept {
	switch (m_typeKind) {
		case TypeKind::I32: return 4;
		case TypeKind::UNIT: return 0;
		case TypeKind::BASIC_BLOCK: return 8;
	default: unreachable();
	}
}

utf::String cir::Type::toString() const {
	switch (m_typeKind) {
		case TypeKind::UNIT: return "unit";
		case TypeKind::I32: return "i32";
		case TypeKind::BASIC_BLOCK: return "bb";
	default: unreachable();
	}
}

