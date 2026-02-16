cmake_minimum_required(VERSION 3.29)

project(kaleidoscope VERSION 1.0)

add_executable(kaleidoscopec
        src/lexer.cpp
        src/main.cpp
        src/ast.cpp
        src/codegen.cpp
        src/errors.cpp
        src/parser.cpp
)

set(LLVM_AREA $ENV{HOME}/repos/llvm-project)

set(LLVM_INCLUDE_LLVM ${LLVM_AREA}/llvm/include/llvm)
set(LLVM_INCLUDE ${LLVM_AREA}/llvm/include)
set(LLVM_BUILD_INCLUDE ${LLVM_AREA}/build/include)

target_include_directories(kaleidoscopec
        PRIVATE ${LLVM_INCLUDE_LLVM} ${LLVM_INCLUDE} ${LLVM_BUILD_INCLUDE})

set(LLVM_LINK_LIBRARIES
        Core
        Support
        Demangle
        BinaryFormat
        Remarks
        BitstreamReader
        TargetParser
)

foreach(LLVMLIB ${LLVM_LINK_LIBRARIES})
    set(LLVMLIB_NAME LLVM${LLVMLIB})
    set(LLVMLIB_FULL_NAME lib${LLVMLIB_NAME}.a)
    message(STATUS "LLVM Lib: ${LLVMLIB} -> ${LLVMLIB_NAME}")
    add_library(${LLVMLIB_NAME} STATIC IMPORTED)
    set_target_properties(${LLVMLIB_NAME} PROPERTIES
            IMPORTED_LOCATION ${LLVM_AREA}/build/lib/${LLVMLIB_FULL_NAME}
    )
    target_link_libraries(kaleidoscopec
            PRIVATE ${LLVMLIB_NAME}
    )
endforeach()

enable_testing()

add_subdirectory(test)
