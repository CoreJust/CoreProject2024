// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Target.hpp"
#include <format>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/TargetParser/Triple.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Target/TargetMachine.h>
#include "error/ErrorPrinter.hpp"

compiler::Target::Target(utf::String targetTripleStr, uint32_t intSize, uint32_t longSize, uint32_t pointerSize)
	: m_intSize(intSize), m_longSize(longSize), m_pointerSize(pointerSize) {
	llvm::Triple targetTriple { targetTripleStr };
	m_architecture = llvm::Triple::getArchTypeName(targetTriple.getArch());
	m_subArchitecture = llvm::Triple::getArchName(targetTriple.getArch(), targetTriple.getSubArch());
	m_vendor = llvm::Triple::getVendorTypeName(targetTriple.getVendor());
	m_OS = llvm::Triple::getOSTypeName(targetTriple.getOS());
	m_environment = llvm::Triple::getEnvironmentTypeName(targetTriple.getEnvironment());

	if (targetTriple.isArch16Bit()) {
		m_architectureBits = 16;

		if (targetTriple.isOSWindows()) { // Win16
			trySetTypeSizes(2, 4, 4);
		} else {
			trySetTypeSizes(2, 2, 2);
		}
	} else if (targetTriple.isArch32Bit()) {
		m_architectureBits = 32;
		trySetTypeSizes(4, 4, 4);
	} else if (targetTriple.isArch64Bit()) {
		m_architectureBits = 64;

		if (targetTriple.isOSWindows()) { // Win64
			trySetTypeSizes(4, 4, 8);
		} else {
			trySetTypeSizes(4, 8, 8);
		}
	} else {
		// Default values.
		m_architectureBits = 64;
		trySetTypeSizes(4, 4, 8);
	}

	m_endianness = targetTriple.isLittleEndian() ? TargetEndianness::LITTLE_ENDIAN : TargetEndianness::BIG_ENDIAN;
}

void compiler::Target::trySetTypeSizes(uint32_t intSize, uint32_t longSize, uint32_t pointerSize) {
	m_intSize = m_intSize ? m_intSize : intSize;
	m_longSize = m_longSize ? m_longSize : longSize;
	m_pointerSize = m_pointerSize ? m_pointerSize : pointerSize;
}

compiler::Target::Target() 
	: Target(llvm::sys::getDefaultTargetTriple()) { }

compiler::Target::Target(utf::String architecture, utf::String vendor, utf::String OS, utf::String environment, uint32_t intSize, uint32_t longSize, uint32_t pointerSize)
	: Target(llvm::Triple::normalize(std::format("{}-{}-{}-{}", architecture, vendor, OS, environment)), intSize, longSize, pointerSize) { }

utf::String compiler::Target::makeLLVMTargetTriple() const noexcept {
	return std::format("{}-{}-{}-{}", m_subArchitecture, m_vendor, m_OS, m_environment);
}

utf::StringView compiler::Target::getArchitecture() const noexcept {
	return m_architecture;
}

utf::StringView compiler::Target::getSubArchitecture() const noexcept {
	return m_subArchitecture;
}

utf::StringView compiler::Target::getVendor() const noexcept {
	return m_vendor;
}

utf::StringView compiler::Target::getOS() const noexcept {
	return m_OS;
}

utf::StringView compiler::Target::getEnvironment() const noexcept {
	return m_environment;
}

uint32_t compiler::Target::getArchitectureBits() const noexcept {
	return m_architectureBits;
}

compiler::TargetEndianness compiler::Target::getEndianness() const noexcept {
	return m_endianness;
}

bool compiler::Target::isLittleEndian() const noexcept {
	return m_endianness == TargetEndianness::LITTLE_ENDIAN;
}

bool compiler::Target::isBigEndian() const noexcept {
	return m_endianness == TargetEndianness::BIG_ENDIAN;
}

uint32_t compiler::Target::getIntSize() const noexcept {
	return m_intSize;
}

uint32_t compiler::Target::getLongSize() const noexcept {
	return m_longSize;
}

uint32_t compiler::Target::getPointerSize() const noexcept {
	return m_pointerSize;
}

const llvm::Target* compiler::Target::makeLLVMTarget() const {
	utf::String targetLookupError;
	utf::String targetTriple = makeLLVMTargetTriple();
	const llvm::Target* llvmTarget = llvm::TargetRegistry::lookupTarget(targetTriple, targetLookupError);
	if (llvmTarget == nullptr) {
		error::ErrorPrinter::fatalError({
			.code = error::ErrorCode::BAD_TARGET,
			.name = "Project error: Bad target",
			.selectionStart = utils::TextPosition(),
			.selectionLength = 0,
			.description = std::format(
				"Failed to initialize target {}: {}.",
				targetTriple,
				targetLookupError
			),
			.explanation = "-"
		});
	}

	return llvmTarget;
}

llvm::TargetMachine* compiler::Target::makeLLVMTargetMachine() const {
	utf::String targetLookupError;
	utf::String targetTriple = makeLLVMTargetTriple();
	const llvm::Target* llvmTarget = llvm::TargetRegistry::lookupTarget(targetTriple, targetLookupError);
	if (llvmTarget == nullptr) {
		error::ErrorPrinter::fatalError({
			.code = error::ErrorCode::BAD_TARGET,
			.name = "Project error: Bad target",
			.selectionStart = utils::TextPosition(),
			.selectionLength = 0,
			.description = std::format(
				"Failed to initialize target {}: {}.",
				targetTriple,
				targetLookupError
			),
			.explanation = "-"
		});
	}

	llvm::TargetOptions targetOptions;
	std::optional<llvm::Reloc::Model> RM = std::optional<llvm::Reloc::Model>();
	return llvmTarget->createTargetMachine(targetTriple, "generic", "", targetOptions, RM);
}
