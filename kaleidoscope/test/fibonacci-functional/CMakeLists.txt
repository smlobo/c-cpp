add_test(NAME fibonacci-functional
        COMMAND $<TARGET_FILE:kaleidoscopec> ${CMAKE_CURRENT_SOURCE_DIR}/fibonacci-functional.ks
)
set_tests_properties(fibonacci-functional PROPERTIES
        FAIL_REGULAR_EXPRESSION "Error"
)

add_test(NAME fibonacci-functional-link
        COMMAND llvm-link fibonacci-functional.ll ${CMAKE_CURRENT_SOURCE_DIR}/test.ll -o linked.bc
)
set_tests_properties(fibonacci-functional-link PROPERTIES DEPENDS fibonacci-functional)

add_test(NAME fibonacci-functional-execute
        COMMAND lli linked.bc
)
set_tests_properties(fibonacci-functional-execute PROPERTIES
        DEPENDS fibonacci-functional-link
        PASS_REGULAR_EXPRESSION "fibonacci functional \\[3] = 2"
        PASS_REGULAR_EXPRESSION "fibonacci functional \\[4] = 3"
        PASS_REGULAR_EXPRESSION "fibonacci functional \\[5] = 5"
        PASS_REGULAR_EXPRESSION "fibonacci functional \\[6] = 8"
)
