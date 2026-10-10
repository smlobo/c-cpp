cmake_minimum_required(VERSION 3.29)

project(mlir-toy VERSION 1.0 LANGUAGES C CXX)

find_package(ZLIB REQUIRED CONFIG)
find_package(zstd REQUIRED CONFIG)
# The LLVM build exports a shared zstd target; Conan supplies the static
# variant of the same dependency to this project.
if(NOT TARGET zstd::libzstd_shared)
    add_library(zstd::libzstd_shared INTERFACE IMPORTED)
    target_link_libraries(zstd::libzstd_shared INTERFACE zstd::libzstd_static)
endif()

add_executable(mlir-toyc
        src/lexer.cpp
        src/main.cpp
        src/ast.cpp
        src/errors.cpp
        src/parser.cpp
        src/Dialect.cpp
        src/MLIRGen.cpp
        src/SimplifyRedundantTranspose.cpp
)

target_compile_features(mlir-toyc PRIVATE cxx_std_20)

# Since llvm by default is compiled without RTTI
target_compile_options(mlir-toyc PRIVATE -fno-rtti)

set(LLVM_AREA $ENV{HOME}/repos/llvm-project)
set(LLVM_BUILD_BIN ${LLVM_AREA}/build/bin)

# Use the LLVM build's dependency list instead of maintaining an incomplete
# hand-written list of LLVM archives.
execute_process(
        COMMAND ${LLVM_BUILD_BIN}/llvm-config --libnames core support
        OUTPUT_VARIABLE LLVM_LIBNAMES
        OUTPUT_STRIP_TRAILING_WHITESPACE
        COMMAND_ERROR_IS_FATAL ANY
)
separate_arguments(LLVM_LIBNAMES NATIVE_COMMAND "${LLVM_LIBNAMES}")

# MLIR's exported targets carry the transitive MLIR dependencies of these
# libraries, including dependencies that change between LLVM revisions.
find_package(MLIR REQUIRED CONFIG
        PATHS ${LLVM_AREA}/build/lib/cmake/mlir
        NO_DEFAULT_PATH)

set(LLVM_INCLUDE ${LLVM_AREA}/llvm/include)
set(LLVM_BUILD_INCLUDE ${LLVM_AREA}/build/include)
set(MLIR_INCLUDE ${LLVM_AREA}/mlir/include)
set(MLIR_BUILD_INCLUDE ${LLVM_AREA}/build/tools/mlir/include)

target_include_directories(mlir-toyc
        PRIVATE ${LLVM_INCLUDE} ${LLVM_BUILD_INCLUDE} ${MLIR_INCLUDE}
                ${MLIR_BUILD_INCLUDE} ${CMAKE_CURRENT_BINARY_DIR}/include)

set(MLIR_LINK_LIBRARIES
        MLIRSupport
        MLIRIR
        MLIRFunctionInterfaces
        MLIRCallInterfaces
        MLIRPass
        MLIRTransforms
        MLIRTransformUtils
        MLIRControlFlowInterfaces
        MLIRRewrite
        MLIRPDLToPDLInterp
        MLIRPDLInterpDialect
        MLIRPDLDialect
        MLIRRewritePDL
        MLIRAnalysis
        MLIRSideEffectInterfaces
)

foreach(LLVMLIB ${LLVM_LIBNAMES})
    target_link_libraries(mlir-toyc PRIVATE ${LLVM_AREA}/build/lib/${LLVMLIB})
endforeach()

target_link_libraries(mlir-toyc PRIVATE ${MLIR_LINK_LIBRARIES})

target_link_libraries(mlir-toyc PRIVATE ZLIB::ZLIB)
target_link_libraries(mlir-toyc PRIVATE zstd::libzstd_static)

add_subdirectory(include)

enable_testing()

add_subdirectory(test)
