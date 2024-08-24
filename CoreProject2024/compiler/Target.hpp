// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	Target.hpp by CoreJust
*	Created on 09.08.2024
*	Contains Target class that contains some information on
*	the compilation target, such as the architecture or the
*	endianness.
*/

#pragma once
#include "utf/String.hpp"

namespace llvm {
	class Target;
	class TargetMachine;
}

namespace compiler {
	enum class TargetEndianness : unsigned char {
		LITTLE_ENDIAN = 0,
		BIG_ENDIAN,

		TARGET_ENDIANNESSES_COUNT
	};

	/*
	*	The Core target depends on LLVM targets, so most of the fields are defined as 
	*	Strings that are fed into LLVM and some information then gets loaded back
	*	(e.g. endianness and architecture bits).
	*/
	class Target final {
	private:
		utf::String m_architecture = "unknown";
		utf::String m_subArchitecture = "unknown";
		utf::String m_vendor = "unknown";
		utf::String m_OS = "unknown";
		utf::String m_environment = "unknown";

		uint32_t m_architectureBits = 64;
		TargetEndianness m_endianness = TargetEndianness::LITTLE_ENDIAN;

		// Those are fields normally derived from the target and you are required to explicitly state them only for some rare architectures.

		// The size of the cint/cuint types in bytes.
		uint32_t m_intSize = 0;

		// The size of the clong/culong types in bytes.
		uint32_t m_longSize = 0;

		// The size of the pointer and isize/usize types in bytes.
		uint32_t m_pointerSize = 0;

	private:
		Target(utf::String targetTripleStr, uint32_t intSize = 0, uint32_t longSize = 0, uint32_t pointerSize = 0);

		// Sets the type sizes if their were not set previously.
		void trySetTypeSizes(uint32_t intSize, uint32_t longSize, uint32_t pointerSize);

	public:
		Target(); // Gets the current architecture
		Target(utf::String architecture, utf::String vendor, utf::String OS, utf::String environment, uint32_t intSize = 0, uint32_t longSize = 0, uint32_t pointerSize = 0);

		utf::String makeLLVMTargetTriple() const noexcept;

		utf::StringView getArchitecture() const noexcept;
		utf::StringView getSubArchitecture() const noexcept;
		utf::StringView getVendor() const noexcept;
		utf::StringView getOS() const noexcept;
		utf::StringView getEnvironment() const noexcept;
		uint32_t getArchitectureBits() const noexcept;
		TargetEndianness getEndianness() const noexcept;

		bool isLittleEndian() const noexcept;
		bool isBigEndian() const noexcept;

		uint32_t getIntSize() const noexcept;
		uint32_t getLongSize() const noexcept;
		uint32_t getPointerSize() const noexcept;

		const llvm::Target* makeLLVMTarget() const;
		llvm::TargetMachine* makeLLVMTargetMachine() const;
	};
}