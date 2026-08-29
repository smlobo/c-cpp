cmake_minimum_required(VERSION 3.29)

project(mlir-toy VERSION 1.0 LANGUAGES CXX)

find_package(ZLIB REQUIRED CONFIG)
find_package(zstd REQUIRED CONFIG)

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

set(LLVM_INCLUDE ${LLVM_AREA}/llvm/include)
set(LLVM_BUILD_INCLUDE ${LLVM_AREA}/build/include)
set(MLIR_INCLUDE ${LLVM_AREA}/mlir/include)
set(MLIR_BUILD_INCLUDE ${LLVM_AREA}/build/tools/mlir/include)

target_include_directories(mlir-toyc
        PRIVATE ${LLVM_INCLUDE} ${LLVM_BUILD_INCLUDE} ${MLIR_INCLUDE}
                ${MLIR_BUILD_INCLUDE} ${CMAKE_CURRENT_BINARY_DIR}/include)

set(LLVM_LINK_LIBRARIES
        LLVMCore
        LLVMSupport
        LLVMDemangle
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

foreach(LLVMLIB_NAME ${LLVM_LINK_LIBRARIES})
    set(LLVMLIB_FULL_NAME lib${LLVMLIB_NAME}.a)
    message(STATUS "LLVM dependency: ${LLVMLIB_NAME}")
    add_library(${LLVMLIB_NAME} STATIC IMPORTED)
    set_target_properties(${LLVMLIB_NAME} PROPERTIES
            IMPORTED_LOCATION ${LLVM_AREA}/build/lib/${LLVMLIB_FULL_NAME}
    )
    target_link_libraries(mlir-toyc
            PRIVATE ${LLVMLIB_NAME}
    )
endforeach()

target_link_libraries(mlir-toyc PRIVATE ZLIB::ZLIB)
target_link_libraries(mlir-toyc PRIVATE zstd::libzstd_static)

add_subdirectory(include)

enable_testing()

add_subdirectory(test)
