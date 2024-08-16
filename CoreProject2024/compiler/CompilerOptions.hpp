// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

/*
*	CompilerOptions.hpp by CoreJust
*	Created on 09.08.2024
*	Contains CompilerOptions class that contains a number of compilation options
*	related to the currently compiled project and used by the compiler.
*	Apart from that, contains some auxiliary declarations for some specific options.
*/

#pragma once
#include "Target.hpp"

namespace compiler {
	enum class ProjectMode : unsigned char {
		/*
		*	The project mode used to build an executable file.
		*	Requires the main function to be defined.
		*/
		EXECUTABLE = 0,

		/*
		*	The project mode used to build a static library that
		*	will be used in other project.
		*/
		STATIC_LIBRARY,

		PROJECT_MODES_COUNT
	};

	/*
	*	EmitMode allows to specify what the compiler must emit as the 
	*	result of its work (AST, CHIR, CIR, LLVM IR, Object files, etc).
	* 
	*	EmitMode is a bit flag.
	*/
	enum class EmitMode : uint32_t {
		NONE = 0x0,
		AST = 0x1,
		CHIR = 0x2,
		CIR = 0x4,
		OPTIMIZED_CIR = 0x8,
		LLVM_IR = 0x10,
		OPTIMIZED_LLVM_IR = 0x20,
		ASSEMBLY = 0x40,
		OBJECT_FILE = 0x80,
		EXECUTABLE_FILE = 0x100, // Includes OBJECT_FILE
		EXECUTION_RESULTS = 0x200, // Includes OBJECT_FILE and EXECUTABLE_FILE

		ALL = 0x3ff
	};

	/*
	*	Compilation mode is a simple way to set up a number of other compiler options in one setting.
	*	It can be used with the other compiler options, and it will have lower precedence:
	*	the compilation mode sets the other options up first, and all the other user options are
	*	applied afterwards.
	*/
	enum class CompilationMode : unsigned char {
		/*
		*	Release is the compilation mode expected to be normally used for
		*	the final version of the project.
		* 
		*	It gets well-optimized and doesn't preserve any debug info.
		*/
		RELEASE = 0,

		/*
		*	Debug mode is expected to be used during development to check if
		*	there are any errors as well as to look for errors source.
		*/
		DEBUG,

		/*
		*	Same as the Debug mode, but uses absolutely no optimizations and involves
		*	some costly runtime checks.
		*/
		HIGH_DEBUG,

		/*
		*	A specific kind of compilation mode where some debugging is required
		*	but the application must execute fast enough for some reason.
		*	It has the execution speed close to that of the Release mode,
		*	but with a lot of debug info preserved as well as some fast checks.
		*/
		RELEASE_WITH_DEBUG_INFO,

		/*
		*	A specific kind of compilation mode where the minimal executable file
		*	size is required.
		*	It is similar to the Release mode, but with the high-most priority
		*	on minimizing the file size.
		*/
		MIN_SIZE_RELEASE,

		COMPILATION_MODES_COUNT
	};

	enum class OptimizationLevel : unsigned char {
		/*
		*	O0 optimization level means that no optimizations are applied at all.
		*	The compilation results in exactly what the programer has written without
		*	anu modifications by the compiler.
		* 
		*	It is the default optimization level in the HighDebug mode.
		*/
		O0 = 0,

		/*
		*	O1 optimization level means that only some quick and simple optimizations
		*	are applied. The general logic of the program is not changed, so this level
		*	can be safely used in the Debug mode.
		* 
		*	It is default in the Debug mode.
		*/
		O1,

		/*
		*	O2 optimization level persecutes the fastest execution while keeping the compilation
		*	time and the resulting file size as small as possible.
		* 
		*	It is the default optimization level in the Release and ReleaseWithDebugInfo modes.
		*/
		O2,

		/*
		*	O3 optimization level persecutes the fastest execution speed at any costs.
		*	The compilation time may be much bigger than in the other levels,
		*	and the resulting file size may grow large, while the program logic might be
		*	strongly changed and become hard to debug.
		*/
		O3,

		/*
		*	OSize optimization level persecutes the minimal resulting file size at any costs.
		*	
		*	It is the default level in the MinSizeRelease mode.
		*/
		OSIZE,

		OPTIMIZATION_LEVELS_COUNT
	};

	class CompilerOptions final {
	private:
		static utf::String s_sourceName;
		static utf::String s_linker;
		static Target s_target;
		static ProjectMode s_projectMode;
		static EmitMode s_emitMode;
		static CompilationMode s_compilationMode;
		static OptimizationLevel s_optimizationLevel;

	public:
		static void loadCompilerOptions();

		static const utf::String& getSourceName() noexcept;
		static const utf::String& getLinker() noexcept;
		static const Target& getTarget() noexcept;
		static ProjectMode getProjectMode() noexcept;
		static EmitMode getEmitMode() noexcept;
		static CompilationMode getCompilationMode() noexcept;
		static OptimizationLevel getOptimizationLevel() noexcept;

		static bool isExecutableMode() noexcept;
		static bool isLibraryMode() noexcept;
		static bool isRelease() noexcept;
		static bool isDebug() noexcept;
		static bool isHighDebug() noexcept;
		static bool isReleaseWithDebugInfo() noexcept;
		static bool isMinSizeRelease() noexcept;

		static bool shallEmitAST() noexcept;
		static bool shallEmitCHIR() noexcept;
		static bool shallEmitCIR() noexcept;
		static bool shallEmitOptimizedCIR() noexcept;
		static bool shallEmitLLVMIR() noexcept;
		static bool shallEmitOptimizedLLVMIR() noexcept;
		static bool shallEmitAssembly() noexcept;
		static bool shallEmitObjectFile() noexcept;
		static bool shallEmitExecutableFile() noexcept;
		static bool shallEmitExecutionResult() noexcept;

		// Returns if the compiler must at least compile until the given phase.
		// It is defined based on whether there is any emitted results on this
		// or later phases.
		static bool shallCompileUpToPhase(EmitMode mode) noexcept;
	};
}
