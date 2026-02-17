add_test(NAME loop
        COMMAND $<TARGET_FILE:kaleidoscopec> ${CMAKE_CURRENT_SOURCE_DIR}/loop.ks
)
set_tests_properties(loop PROPERTIES
        FAIL_REGULAR_EXPRESSION "Error"
)

add_test(NAME loop-link
        COMMAND llvm-link loop.ll ${CMAKE_CURRENT_SOURCE_DIR}/test.ll -o linked.bc
)
set_tests_properties(loop-link PROPERTIES DEPENDS loop)

add_test(NAME loop-execute
        COMMAND lli linked.bc
)
set_tests_properties(loop-execute PROPERTIES
        DEPENDS loop-link
        PASS_REGULAR_EXPRESSION "ZZZZ"
)
