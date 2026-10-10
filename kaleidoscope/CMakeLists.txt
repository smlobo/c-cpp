cmake_minimum_required(VERSION 3.29)

project(kaleidoscope VERSION 1.0 LANGUAGES CXX)

find_package(ZLIB REQUIRED CONFIG)
find_package(zstd REQUIRED CONFIG)

add_executable(kaleidoscopec
        src/lexer.cpp
        src/main.cpp
        src/ast.cpp
        src/codegen.cpp
        src/errors.cpp
        src/parser.cpp
)

target_compile_features(kaleidoscopec PRIVATE cxx_std_20)

set(LLVM_AREA $ENV{HOME}/repos/llvm-project)

set(LLVM_INCLUDE_LLVM ${LLVM_AREA}/llvm/include/llvm)
set(LLVM_INCLUDE ${LLVM_AREA}/llvm/include)
set(LLVM_BUILD_INCLUDE ${LLVM_AREA}/build/include)
set(LLVM_BUILD_BIN ${LLVM_AREA}/build/bin)

target_include_directories(kaleidoscopec
        PRIVATE ${LLVM_INCLUDE_LLVM} ${LLVM_INCLUDE} ${LLVM_BUILD_INCLUDE})

execute_process(
        COMMAND ${LLVM_BUILD_BIN}/llvm-config --libnames native passes
        OUTPUT_VARIABLE LLVM_LIBNAMES
        OUTPUT_STRIP_TRAILING_WHITESPACE
        COMMAND_ERROR_IS_FATAL ANY
)
separate_arguments(LLVM_LIBNAMES NATIVE_COMMAND "${LLVM_LIBNAMES}")
foreach(LLVMLIB ${LLVM_LIBNAMES})
    target_link_libraries(kaleidoscopec PRIVATE ${LLVM_AREA}/build/lib/${LLVMLIB})
endforeach()

target_link_libraries(kaleidoscopec PRIVATE ZLIB::ZLIB)
target_link_libraries(kaleidoscopec PRIVATE zstd::libzstd_static)

enable_testing()

add_subdirectory(test)
