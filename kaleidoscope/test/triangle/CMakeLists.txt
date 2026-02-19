add_test(NAME triangle
        COMMAND $<TARGET_FILE:kaleidoscopec> ${CMAKE_CURRENT_SOURCE_DIR}/triangle.ks
)
set_tests_properties(triangle PROPERTIES
        FAIL_REGULAR_EXPRESSION "Error"
)

add_test(NAME triangle-link
        COMMAND llvm-link triangle.ll ${CMAKE_CURRENT_SOURCE_DIR}/test.ll -o linked.bc
)
set_tests_properties(triangle-link PROPERTIES DEPENDS triangle)

add_test(NAME triangle-execute
        COMMAND lli linked.bc
)
set_tests_properties(triangle-execute PROPERTIES
        DEPENDS triangle-link
        PASS_REGULAR_EXPRESSION "X"
        PASS_REGULAR_EXPRESSION "XX"
        PASS_REGULAR_EXPRESSION "XXX"
        PASS_REGULAR_EXPRESSION "XXXX"
)
