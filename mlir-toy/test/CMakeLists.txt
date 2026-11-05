set(MLIR_TOY_TEST_RUNNER "${CMAKE_CURRENT_SOURCE_DIR}/RunAndCompare.cmake")

function(add_mlir_golden_test test_name)
    # Pass NO_OPTIMIZE for tests whose expected IR should not use -O.
    cmake_parse_arguments(TEST "NO_OPTIMIZE" "" "" ${ARGN})

    set(test_source_dir "${CMAKE_CURRENT_SOURCE_DIR}/${test_name}")
    set(test_binary_dir "${CMAKE_CURRENT_BINARY_DIR}/${test_name}")

    set(test_optimize ON)
    if (TEST_NO_OPTIMIZE)
        set(test_optimize OFF)
    endif()

    add_test(NAME "${test_name}"
            COMMAND "${CMAKE_COMMAND}"
            "-DCOMPILER=$<TARGET_FILE:mlir-toyc>"
            "-DINPUT=${test_source_dir}/${test_name}.mt"
            "-DEXPECTED=${test_source_dir}/${test_name}.mlir"
            "-DOUTPUT=${test_binary_dir}/${test_name}.mlir"
            "-DOPTIMIZE=${test_optimize}"
            -P "${MLIR_TOY_TEST_RUNNER}"
    )
endfunction()

add_mlir_golden_test(return-add)
add_mlir_golden_test(multiply-transpose)
add_mlir_golden_test(transpose-transpose)
add_mlir_golden_test(var-decl NO_OPTIMIZE)
add_mlir_golden_test(print NO_OPTIMIZE)
add_mlir_golden_test(reshape-optimize)
add_mlir_golden_test(generic-call)
