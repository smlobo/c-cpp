add_test(NAME extern
        COMMAND $<TARGET_FILE:kaleidoscopec> ${CMAKE_CURRENT_SOURCE_DIR}/external-function.ks
)
set_tests_properties(extern PROPERTIES
        FAIL_REGULAR_EXPRESSION "Error"
)

add_test(NAME extern-link
        COMMAND llvm-link external-function.ll ${CMAKE_CURRENT_SOURCE_DIR}/test.ll -o linked.bc
)
set_tests_properties(extern-link PROPERTIES DEPENDS extern)

add_test(NAME extern-execute
        COMMAND lli linked.bc
)
set_tests_properties(extern-execute PROPERTIES
        DEPENDS extern-link
        PASS_REGULAR_EXPRESSION "result = 1\.00"
)
