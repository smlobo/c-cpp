set(MLIR_TOY_TEST_RUNNER "${CMAKE_CURRENT_SOURCE_DIR}/RunAndCompare.cmake")

function(add_mlir_golden_test test_name)
    set(test_source_dir "${CMAKE_CURRENT_SOURCE_DIR}/${test_name}")
    set(test_binary_dir "${CMAKE_CURRENT_BINARY_DIR}/${test_name}")

    add_test(NAME "${test_name}"
            COMMAND "${CMAKE_COMMAND}"
            "-DCOMPILER=$<TARGET_FILE:mlir-toyc>"
            "-DINPUT=${test_source_dir}/${test_name}.mt"
            "-DEXPECTED=${test_source_dir}/${test_name}.mlir"
            "-DOUTPUT=${test_binary_dir}/${test_name}.mlir"
            -P "${MLIR_TOY_TEST_RUNNER}"
    )
endfunction()

add_mlir_golden_test(return-add)
add_mlir_golden_test(multiply-transpose)
add_mlir_golden_test(transpose-transpose)
