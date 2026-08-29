foreach(variable COMPILER INPUT EXPECTED OUTPUT)
    if (NOT DEFINED ${variable})
        message(FATAL_ERROR "${variable} must be specified")
    endif()
endforeach()

get_filename_component(output_directory "${OUTPUT}" DIRECTORY)
file(MAKE_DIRECTORY "${output_directory}")
file(REMOVE "${OUTPUT}")

execute_process(
    COMMAND "${COMPILER}" "${INPUT}" -O -a
    WORKING_DIRECTORY "${output_directory}"
    RESULT_VARIABLE compiler_result
)
if (NOT compiler_result EQUAL 0)
    message(FATAL_ERROR "mlir-toyc failed with exit code ${compiler_result}")
endif()

execute_process(
    COMMAND "${CMAKE_COMMAND}" -E compare_files "${OUTPUT}" "${EXPECTED}"
    RESULT_VARIABLE compare_result
)
if (NOT compare_result EQUAL 0)
    message(FATAL_ERROR "Generated MLIR differs from ${EXPECTED}")
endif()
