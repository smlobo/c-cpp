find_program(CC_EXECUTABLE NAMES cc REQUIRED)

function(add_kaleidoscope_test name)
    cmake_parse_arguments(KS "OPTIMIZE" "" "EXPECT" ${ARGN})

    file(GLOB ks_files "${CMAKE_CURRENT_SOURCE_DIR}/*.ks")
    list(LENGTH ks_files ks_file_count)
    if(NOT ks_file_count EQUAL 1)
        message(FATAL_ERROR "${CMAKE_CURRENT_SOURCE_DIR} must contain exactly one .ks file")
    endif()
    list(GET ks_files 0 ks_file)
    get_filename_component(ks_stem "${ks_file}" NAME_WE)

    set(compiler_args)
    if(KS_OPTIMIZE)
        list(APPEND compiler_args -O)
    endif()

    add_test(NAME ${name}
            COMMAND $<TARGET_FILE:kaleidoscopec> ${compiler_args} ${ks_file}
    )
    set_tests_properties(${name} PROPERTIES
            FAIL_REGULAR_EXPRESSION "Error"
            FIXTURES_SETUP "${name}-compiled"
    )

    add_test(NAME ${name}-link
            COMMAND ${LLVM_BUILD_BIN}/llvm-link ${ks_stem}.ll
                    ${CMAKE_CURRENT_SOURCE_DIR}/test.ll -o linked.bc
    )
    set_tests_properties(${name}-link PROPERTIES
            FIXTURES_REQUIRED "${name}-compiled"
            FIXTURES_SETUP "${name}-linked"
    )

    add_test(NAME ${name}-execute
            COMMAND ${LLVM_BUILD_BIN}/lli linked.bc
    )
    set_tests_properties(${name}-execute PROPERTIES
            FIXTURES_REQUIRED "${name}-linked"
    )

    add_test(NAME ${name}-test-assembly
            COMMAND ${LLVM_BUILD_BIN}/llc ${CMAKE_CURRENT_SOURCE_DIR}/test.ll -o test.s
    )
    set_tests_properties(${name}-test-assembly PROPERTIES
            FIXTURES_SETUP "${name}-test-assembly-ready"
    )

    add_test(NAME ${name}-assembly-link
            COMMAND ${CC_EXECUTABLE} ${ks_stem}.s test.s -o a.out
    )
    set_tests_properties(${name}-assembly-link PROPERTIES
            FIXTURES_REQUIRED "${name}-compiled;${name}-test-assembly-ready"
            FIXTURES_SETUP "${name}-assembly-linked"
    )

    add_test(NAME ${name}-assembly-execute
            COMMAND ${CMAKE_CURRENT_BINARY_DIR}/a.out
    )
    set_tests_properties(${name}-assembly-execute PROPERTIES
            FIXTURES_REQUIRED "${name}-assembly-linked"
    )

    if(KS_EXPECT)
        set_tests_properties(${name}-execute ${name}-assembly-execute PROPERTIES
                PASS_REGULAR_EXPRESSION "${KS_EXPECT}"
        )
    endif()
endfunction()

add_subdirectory(basic)
add_subdirectory(cse)
add_subdirectory(extern)
add_subdirectory(condition)
add_subdirectory(loop)
add_subdirectory(triangle)
add_subdirectory(library-sequence)
add_subdirectory(mandelbrot)
add_subdirectory(fibonacci-functional)
add_subdirectory(mutate)
add_subdirectory(fibonacci-iterative)
