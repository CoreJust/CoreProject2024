// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Type.hpp"
#include <cassert>

symbol::Type::Type(TypeKind typeKind) noexcept : m_typeKind(typeKind) {

}

cir::Type symbol::Type::makeCirType() const {
	assert(m_typeKind != TypeKind::NO_TYPE);

	switch (m_typeKind) {
		case TypeKind::I32: return cir::TypeKind::I32;
		case TypeKind::UNIT: [[fallthrough]];
		case TypeKind::NEVER_TYPE: return cir::TypeKind::UNIT;
	default: unreachable();
	}
}

bool symbol::Type::operator==(const Type& other) const noexcept {
	return m_typeKind == other.getTypeKind();
}

symbol::TypeKind symbol::Type::getTypeKind() const noexcept {
	return m_typeKind;
}

uint32_t symbol::Type::getTypeSize() const noexcept {
	assert(m_typeKind != TypeKind::NO_TYPE);

	switch (m_typeKind) {
		case TypeKind::I32: return 4;
		case TypeKind::UNIT: [[fallthrough]];
		case TypeKind::NEVER_TYPE: return 0;
	default: unreachable();
	}
}

utf::String symbol::Type::toString() const {
	assert(m_typeKind != TypeKind::NO_TYPE);

	switch (m_typeKind) {
		case TypeKind::UNIT: return "unit";
		case TypeKind::I32: return "i32";
		case TypeKind::NEVER_TYPE: return "never";
	default: unreachable();
	}
}

utf::String symbol::Type::toMangleString() const {
	assert(m_typeKind != TypeKind::NO_TYPE);

	switch (m_typeKind) {
		case TypeKind::UNIT: return "u";
		case TypeKind::I32: return "i4";
		case TypeKind::NEVER_TYPE: return "n";
	default: unreachable();
	}
}

bool symbol::Type::isNoType() const noexcept {
	return m_typeKind == TypeKind::NO_TYPE;
}
