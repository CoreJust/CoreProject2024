// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "ChirConstantValue.hpp"
#include "utf/FastFmt.hpp" // For ilog2
#include "error/ErrorPrinter.hpp"

const utils::IntValue INT_LIMITS[][2] = { // [ilog2(bytes) - 1][is_signed]
	{ utils::IntValue(0xff), utils::IntValue(0x7f) }, // u8, i8
	{ utils::IntValue(0xffff), utils::IntValue(0x7fff) }, // u16, i16
	{ utils::IntValue(0xffffffff), utils::IntValue(0x7fffffff) }, // u32, i32
	{ utils::IntValue(0xffffffffffffffff), utils::IntValue(0x7fffffffffffffff) }, // u64, i64
	{ utils::IntValue("ffffffffffffffffffffffffffffffff", 16), utils::IntValue("7fffffffffffffffffffffffffffffff", 16) }, // u128, i128
};

const bool canTypeHoldIntValue(utils::NoNull<symbol::Type> type, utils::IntValue& value) noexcept {
	utils::IntValue maxValue = INT_LIMITS[utf::ilog2(type->getTypeSize()) - 1][type->isSignedType()];
	if (type->isSignedType() && value >= utils::IntValue(0)) {
		maxValue -= utils::IntValue(1);
	}

	return utils::abs(value) <= maxValue;
}

chir::ConstantValue::ConstantValue(utils::NoNull<symbol::Type> type, utils::IntValue value) noexcept
	: Value(NodeKind::CONSTANT_VALUE, type), m_value(value) { }

void chir::ConstantValue::setIntLiteralType(utils::NoNull<symbol::Type> type) noexcept {
	if (!type->isSignedType() && m_value < 0) {
		error::ErrorPrinter::fatalError({
			.code = error::ErrorCode::IMPOSIBLE_INT_LITERAL,
			.name = "Semantic error: Assigning negative value to unsigned type",
			.selectionStart = m_position,
			.selectionLength = 1,
			.description = "Tried to use a negative integer literal in a context where an unsigned value is expected."
		});
	}

	if (!canTypeHoldIntValue(type, m_value)) {
		error::ErrorPrinter::fatalError({
			.code = error::ErrorCode::IMPOSIBLE_INT_LITERAL,
			.name = "Semantic error: Int literal overflow",
			.selectionStart = m_position,
			.selectionLength = 1,
			.description = "Tried to assign a value that is larger than the type can hold."
		});
	}

	m_type = type;
}

void chir::ConstantValue::assignOwnLiteralType() noexcept {
	const static utils::NoNull<symbol::Type> DEFAULT_TYPES[] = {
		symbol::Type::make(symbol::TypeKind::I32), 
		symbol::Type::make(symbol::TypeKind::I64),
		symbol::Type::make(symbol::TypeKind::U64), 
		symbol::Type::make(symbol::TypeKind::I128), 
		symbol::Type::make(symbol::TypeKind::U128)
	};

	for (auto type : DEFAULT_TYPES) {
		if (!type->isSignedType() && m_value < 0) continue;
		if (!canTypeHoldIntValue(type, m_value)) continue;

		m_type = type;
		return;
	}

	error::internalAssert(false, "Cannot fill the value in any type.");
}

void chir::ConstantValue::setBoolLiteralType() noexcept {
	error::internalAssert(m_value == 0 || m_value == 1);

	m_type = symbol::Type::make(symbol::TypeKind::BOOL);
}

utils::IntValue chir::ConstantValue::getValue() const noexcept {
	return m_value;
}

utils::IntValue& chir::ConstantValue::getValueAsRef() noexcept {
	return m_value;
}
