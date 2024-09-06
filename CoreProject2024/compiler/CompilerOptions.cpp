// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "CompilerOptions.hpp"

utf::String compiler::CompilerOptions::s_sourceName = "bench";
utf::String compiler::CompilerOptions::s_linker = "clang++";
compiler::Target compiler::CompilerOptions::s_target { };
compiler::ProjectMode compiler::CompilerOptions::s_projectMode = compiler::ProjectMode::EXECUTABLE;
compiler::EmitMode compiler::CompilerOptions::s_emitMode = compiler::EmitMode::OBJECT_FILE;
compiler::CompilationMode compiler::CompilerOptions::s_compilationMode = compiler::CompilationMode::RELEASE;
compiler::OptimizationLevel compiler::CompilerOptions::s_optimizationLevel = compiler::OptimizationLevel::O2;

void compiler::CompilerOptions::loadCompilerOptions() {
	// TMP, loading from the project file will be added later.
}

const utf::String& compiler::CompilerOptions::getSourceName() noexcept {
	return s_sourceName;
}

const utf::String& compiler::CompilerOptions::getLinker() noexcept {
	return s_linker;
}

const compiler::Target& compiler::CompilerOptions::getTarget() noexcept {
	return s_target;
}

compiler::ProjectMode compiler::CompilerOptions::getProjectMode() noexcept {
	return s_projectMode;
}

compiler::EmitMode compiler::CompilerOptions::getEmitMode() noexcept {
	return s_emitMode;
}

compiler::CompilationMode compiler::CompilerOptions::getCompilationMode() noexcept {
	return s_compilationMode;
}

compiler::OptimizationLevel compiler::CompilerOptions::getOptimizationLevel() noexcept {
	return s_optimizationLevel;
}

bool compiler::CompilerOptions::isExecutableMode() noexcept {
	return s_projectMode == ProjectMode::EXECUTABLE;
}

bool compiler::CompilerOptions::isLibraryMode() noexcept {
	return s_projectMode == ProjectMode::STATIC_LIBRARY;
}

bool compiler::CompilerOptions::isRelease() noexcept {
	return s_compilationMode == CompilationMode::RELEASE;
}

bool compiler::CompilerOptions::isDebug() noexcept {
	return s_compilationMode == CompilationMode::DEBUG;
}

bool compiler::CompilerOptions::isHighDebug() noexcept {
	return s_compilationMode == CompilationMode::HIGH_DEBUG;
}

bool compiler::CompilerOptions::isReleaseWithDebugInfo() noexcept {
	return s_compilationMode == CompilationMode::RELEASE_WITH_DEBUG_INFO;
}

bool compiler::CompilerOptions::isMinSizeRelease() noexcept {
	return s_compilationMode == CompilationMode::MIN_SIZE_RELEASE;
}

bool compiler::CompilerOptions::shallEmitAST() noexcept {
	return static_cast<uint32_t>(s_emitMode) & static_cast<uint32_t>(EmitMode::AST);
}

bool compiler::CompilerOptions::shallEmitCHIR() noexcept {
	return static_cast<uint32_t>(s_emitMode) & static_cast<uint32_t>(EmitMode::CHIR);
}

bool compiler::CompilerOptions::shallEmitCIR() noexcept {
	return static_cast<uint32_t>(s_emitMode) & static_cast<uint32_t>(EmitMode::CIR);
}

bool compiler::CompilerOptions::shallEmitOptimizedCIR() noexcept {
	return static_cast<uint32_t>(s_emitMode) & static_cast<uint32_t>(EmitMode::OPTIMIZED_CIR);
}

bool compiler::CompilerOptions::shallEmitLLVMIR() noexcept {
	return static_cast<uint32_t>(s_emitMode) & static_cast<uint32_t>(EmitMode::LLVM_IR);
}

bool compiler::CompilerOptions::shallEmitOptimizedLLVMIR() noexcept {
	return static_cast<uint32_t>(s_emitMode) & static_cast<uint32_t>(EmitMode::OPTIMIZED_LLVM_IR);
}

bool compiler::CompilerOptions::shallEmitAssembly() noexcept {
	return static_cast<uint32_t>(s_emitMode) & static_cast<uint32_t>(EmitMode::ASSEMBLY);
}

bool compiler::CompilerOptions::shallEmitObjectFile() noexcept {
	return static_cast<uint32_t>(s_emitMode) & static_cast<uint32_t>(EmitMode::OBJECT_FILE);
}

bool compiler::CompilerOptions::shallEmitExecutableFile() noexcept {
	return static_cast<uint32_t>(s_emitMode) & static_cast<uint32_t>(EmitMode::EXECUTABLE_FILE);
}

bool compiler::CompilerOptions::shallEmitExecutionResult() noexcept {
	return static_cast<uint32_t>(s_emitMode) & static_cast<uint32_t>(EmitMode::EXECUTION_RESULTS);
}

bool compiler::CompilerOptions::shallCompileUpToPhase(EmitMode mode) noexcept {
	return static_cast<uint32_t>(s_emitMode) & ~(static_cast<uint32_t>(mode) - 1);
}
