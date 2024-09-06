// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CirValue.hpp by CoreJust
*	Created on 05.08.2024
*	Contains Value class that is base for all values in CIR, that is, all objects
*	that can be used as arguments for CIR instructions.
*/

#pragma once
#include <vector>
#include "utils/NoNull.hpp"
#include "../type/CirType.hpp"
#include "CirValueId.hpp"
#include "CirValueKind.hpp"

namespace cir {
	/*
	*	Almost everything in CIR is a value (apart from types).
	*	There are 2 general kinds of Value:
	*	1) Constant - compile-time known value (a literal or a symbol/basic block).
	*	2) Instruction - an operation that makes up the code and uses other values.
	*	3) FunctionArgument - special value used for function arguments 
	*	   (since they cannot be added to any of the aforementioned categories).
	*/
	class Value {
	protected:
		std::vector<utils::NoNull<Value>> m_users; // List of instructions that use this value.
		utf::String m_name;
		utils::NoNull<Type> m_type;
		ValueId m_id;
		ValueKind m_kind;

	public:
		Value(utf::String name, utils::NoNull<Type> type, ValueKind kind) noexcept;
		virtual ~Value() noexcept = default;

		utf::String getName() const noexcept;
		utils::NoNull<Type> getType() const noexcept;
		const ValueId getId() const noexcept;
		const ValueKind getKind() const noexcept;
		std::vector<utils::NoNull<Value>>& getUsers() noexcept;

		bool isConstant() const noexcept;
		bool isGlobalValue() const noexcept;
		bool isFunction() const noexcept;
		bool isNativeFunction() const noexcept;
		bool isCommonFunction() const noexcept;
		bool isGlobalVariable() const noexcept;
		bool isInstruction() const noexcept;
		bool isTerminator() const noexcept;
		bool isUsable() const noexcept;

		// Returns the string representation of the use of the Value!
		utf::String toString() const;

	protected:
		// ! The value must be usable.
		static void addUser(utils::NoNull<Value> value, Value& user);

	private:
		static ValueId generateUniqueId() noexcept;
	};
}
