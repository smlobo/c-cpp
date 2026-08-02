set(TEST_NAME "mutate")

add_test(NAME ${TEST_NAME}
        COMMAND $<TARGET_FILE:kaleidoscopec> ${CMAKE_CURRENT_SOURCE_DIR}/mutate.ks
)
set_tests_properties(${TEST_NAME} PROPERTIES
        FAIL_REGULAR_EXPRESSION "Error"
)

add_test(NAME ${TEST_NAME}-link
        COMMAND llvm-link mutate.ll ${CMAKE_CURRENT_SOURCE_DIR}/test.ll -o linked.bc
)
set_tests_properties(${TEST_NAME}-link PROPERTIES DEPENDS ${TEST_NAME})

add_test(NAME ${TEST_NAME}-execute
        COMMAND lli linked.bc
)
set_tests_properties(${TEST_NAME}-execute PROPERTIES
        DEPENDS ${TEST_NAME}-link
        PASS_REGULAR_EXPRESSION "123\.00"
        PASS_REGULAR_EXPRESSION "4\.00"
)
