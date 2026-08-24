set(OPS_H_INC ${CMAKE_CURRENT_BINARY_DIR}/Ops.h.inc)
set(OPS_CPP_INC ${CMAKE_CURRENT_BINARY_DIR}/Ops.cpp.inc)
set(DIALECT_H_INC ${CMAKE_CURRENT_BINARY_DIR}/Dialect.h.inc)
set(DIALECT_CPP_INC ${CMAKE_CURRENT_BINARY_DIR}/Dialect.cpp.inc)

add_custom_command(
    OUTPUT ${OPS_H_INC}
    COMMAND ${LLVM_AREA}/build/bin/mlir-tblgen
            -gen-op-decls
            ${CMAKE_CURRENT_SOURCE_DIR}/Ops.td
            -I ${LLVM_AREA}/mlir/include
            -o ${OPS_H_INC}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Ops.td
    VERBATIM
)

add_custom_command(
    OUTPUT ${OPS_CPP_INC}
    COMMAND ${LLVM_AREA}/build/bin/mlir-tblgen
            -gen-op-defs
            ${CMAKE_CURRENT_SOURCE_DIR}/Ops.td
            -I ${LLVM_AREA}/mlir/include
            -o ${OPS_CPP_INC}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Ops.td
    VERBATIM
)

add_custom_command(
    OUTPUT ${DIALECT_H_INC}
    COMMAND ${LLVM_AREA}/build/bin/mlir-tblgen
            -gen-dialect-decls
            ${CMAKE_CURRENT_SOURCE_DIR}/Ops.td
            -I ${LLVM_AREA}/mlir/include
            -o ${DIALECT_H_INC}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Ops.td
    VERBATIM
)

add_custom_command(
    OUTPUT ${DIALECT_CPP_INC}
    COMMAND ${LLVM_AREA}/build/bin/mlir-tblgen
            -gen-dialect-defs
            ${CMAKE_CURRENT_SOURCE_DIR}/Ops.td
            -I ${LLVM_AREA}/mlir/include
            -o ${DIALECT_CPP_INC}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Ops.td
    VERBATIM
)

add_custom_target(OpsGen ALL DEPENDS
    ${OPS_H_INC}
    ${OPS_CPP_INC}
    ${DIALECT_H_INC}
    ${DIALECT_CPP_INC}
)

# Make the generated headers explicit target inputs.  Besides preserving the
# build dependency, this lets CMake-aware IDEs (including CLion) discover the
# declarations in Dialect.h.inc while indexing src/main.cpp.
target_sources(mlir-toyc PRIVATE
    ${OPS_H_INC}
    ${OPS_CPP_INC}
    ${DIALECT_H_INC}
    ${DIALECT_CPP_INC}
)

add_dependencies(mlir-toyc OpsGen)
