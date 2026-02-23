add_test(NAME sequence
        COMMAND $<TARGET_FILE:kaleidoscopec> ${CMAKE_CURRENT_SOURCE_DIR}/sequence.ks
)
set_tests_properties(sequence PROPERTIES
        FAIL_REGULAR_EXPRESSION "Error"
)

add_test(NAME sequence-link
        COMMAND llvm-link sequence.ll ${CMAKE_CURRENT_SOURCE_DIR}/test.ll -o linked.bc
)
set_tests_properties(sequence-link PROPERTIES DEPENDS sequence)

add_test(NAME sequence-execute
        COMMAND lli linked.bc
)
set_tests_properties(sequence-execute PROPERTIES
        DEPENDS sequence-link
        PASS_REGULAR_EXPRESSION "123\.00"
        PASS_REGULAR_EXPRESSION "456\.00"
        PASS_REGULAR_EXPRESSION "789\.00"
)
