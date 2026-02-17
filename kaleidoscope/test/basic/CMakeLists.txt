add_test(NAME basic
        COMMAND $<TARGET_FILE:kaleidoscopec> ${CMAKE_CURRENT_SOURCE_DIR}/basic.ks
)
set_tests_properties(basic PROPERTIES
        FAIL_REGULAR_EXPRESSION "Error"
)

add_test(NAME basic-link
        COMMAND llvm-link basic.ll ${CMAKE_CURRENT_SOURCE_DIR}/test.ll -o linked.bc
)
set_tests_properties(basic-link PROPERTIES DEPENDS basic)

add_test(NAME basic-execute
        COMMAND lli linked.bc
)
set_tests_properties(basic-execute PROPERTIES
        DEPENDS basic-link
        PASS_REGULAR_EXPRESSION "result = 6\.00"
)
