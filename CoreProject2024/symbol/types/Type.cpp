// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Type.hpp"
#include <cassert>

symbol::Type::Type(BasicType basicType) noexcept : m_basicType(basicType) {

}

bool symbol::Type::operator==(const Type& other) const noexcept {
	return m_basicType == other.getBasicType();
}

symbol::BasicType symbol::Type::getBasicType() const noexcept {
	return m_basicType;
}

uint32_t symbol::Type::getTypeSize() const noexcept {
	assert(m_basicType != BasicType::NO_TYPE);

	switch (m_basicType) {
		case BasicType::UNIT: return 0;
		case BasicType::I32: return 4;
		case BasicType::NEVER_TYPE: return 0;
	default: unreachable();
	}
}

utf::String symbol::Type::toString() const {
	assert(m_basicType != BasicType::NO_TYPE);

	switch (m_basicType) {
		case BasicType::UNIT: return "unit";
		case BasicType::I32: return "i32";
		case BasicType::NEVER_TYPE: return "never";
	default: unreachable();
	}
}

utf::String symbol::Type::toMangleString() const {
	assert(m_basicType != BasicType::NO_TYPE);

	switch (m_basicType) {
		case BasicType::UNIT: return "u";
		case BasicType::I32: return "i4";
		case BasicType::NEVER_TYPE: return "n";
	default: unreachable();
	}
}

bool symbol::Type::isNoType() const noexcept {
	return m_basicType == BasicType::NO_TYPE;
}
