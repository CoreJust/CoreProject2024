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
	enum class TargetEndianness : uint8_t {
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

	private:
		Target(utf::String targetTripleStr);

	public:
		Target(); // Gets the current architecture
		Target(utf::String architecture, utf::String vendor, utf::String OS, utf::String environment);

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

		const llvm::Target* makeLLVMTarget() const;
		llvm::TargetMachine* makeLLVMTargetMachine() const;
	};
}