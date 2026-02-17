add_test(NAME condition
        COMMAND $<TARGET_FILE:kaleidoscopec> ${CMAKE_CURRENT_SOURCE_DIR}/condition.ks
)
set_tests_properties(condition PROPERTIES
        FAIL_REGULAR_EXPRESSION "Error"
)

add_test(NAME condition-link
        COMMAND llvm-link condition.ll ${CMAKE_CURRENT_SOURCE_DIR}/test.ll -o linked.bc
)
set_tests_properties(condition-link PROPERTIES DEPENDS condition)

add_test(NAME condition-execute
        COMMAND lli linked.bc
)
set_tests_properties(condition-execute PROPERTIES
        DEPENDS condition-link
        PASS_REGULAR_EXPRESSION "result: 3\.00 \-> 5\.00"
        PASS_REGULAR_EXPRESSION "result: \-3\.00 \-> \-6\.00"
)
