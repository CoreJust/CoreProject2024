// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CirValue.hpp"
#include <atomic>
#include "constant/CirConstanNumber.hpp"

cir::Value::Value(utf::String name, Type type, ValueKind kind) noexcept
    : m_name(std::move(name)), m_type(std::move(type)), m_kind(kind), m_id(generateUniqueId()) {
    assert((type == TypeKind::UNIT) == !cir::isUsable(kind));
}

utf::StringView cir::Value::getName() const noexcept {
    return m_name;
}

const cir::Type& cir::Value::getType() const noexcept {
    return m_type;
}

const cir::ValueId cir::Value::getId() const noexcept {
    return m_id;
}

const cir::ValueKind cir::Value::getKind() const noexcept {
    return m_kind;
}

std::vector<utils::NoNull<cir::Value>>& cir::Value::getUsers() noexcept {
    return m_users;
}

bool cir::Value::isConstant() const noexcept {
    return cir::isConstant(m_kind);
}

bool cir::Value::isGlobalValue() const noexcept {
    return cir::isGlobalValue(m_kind);
}

bool cir::Value::isFunction() const noexcept {
    return cir::isFunction(m_kind);
}

bool cir::Value::isNativeFunction() const noexcept {
    return m_kind == ValueKind::NATIVE_FUNCTION;
}

bool cir::Value::isCommonFunction() const noexcept {
    return m_kind == ValueKind::COMMON_FUNCTION;
}

bool cir::Value::isGlobalVariable() const noexcept {
    return m_kind == ValueKind::GLOBAL_VARIABLE;
}

bool cir::Value::isInstruction() const noexcept {
    return cir::isIntruction(m_kind);
}

bool cir::Value::isTerminator() const noexcept {
    return cir::isTerminator(m_kind);
}

bool cir::Value::isUsable() const noexcept {
    return cir::isUsable(m_kind);
}

utf::String cir::Value::toString() const {
    switch (m_kind) {
        case cir::ValueKind::CONSTANT_NUMBER: return std::format("{} {}", m_type.toString(), reinterpret_cast<const ConstantNumber*>(this)->getValue());
        case cir::ValueKind::GLOBAL_VARIABLE: return std::format("{} global {}", m_type.toString(), m_name);
        case cir::ValueKind::COMMON_FUNCTION: [[fallthrough]];
        case cir::ValueKind::NATIVE_FUNCTION: return std::format("fn {}", m_name);
        case cir::ValueKind::BASIC_BLOCK: return std::format("${}", m_name);
        case cir::ValueKind::UNARY_INSTRUCTION: [[fallthrough]];
        case cir::ValueKind::BINARY_INSTRUCTION: [[fallthrough]];
        case cir::ValueKind::INVOCATION_INSTRUCTION: return std::format("{} tmp {}", m_type.toString(), m_name);
        case cir::ValueKind::LOCAL_VARIABLE: [[fallthrough]];
        case cir::ValueKind::FUNCTION_ARGUMENT: return std::format("{} local {}", m_type.toString(), m_name);
    default: return "";
    }
}

void cir::Value::addUser(utils::NoNull<Value> value, Value& user) {
    assert(value->isUsable());

    value->m_users.emplace_back(&user);
}

cir::ValueId cir::Value::generateUniqueId() noexcept {
    static std::atomic<ValueId> s_idCounter { 1 };

    return s_idCounter++;
}
