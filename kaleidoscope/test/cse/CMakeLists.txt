add_test(NAME cse
        COMMAND $<TARGET_FILE:kaleidoscopec> -O ${CMAKE_CURRENT_SOURCE_DIR}/cse.ks
)
set_tests_properties(cse PROPERTIES
        FAIL_REGULAR_EXPRESSION "Error"
)

add_test(NAME cse-link
        COMMAND llvm-link cse.ll ${CMAKE_CURRENT_SOURCE_DIR}/test.ll -o linked.bc
)
set_tests_properties(cse-link PROPERTIES DEPENDS cse)

add_test(NAME cse-execute
        COMMAND lli linked.bc
)
set_tests_properties(cse-execute PROPERTIES
        DEPENDS cse-link
        PASS_REGULAR_EXPRESSION "result = 64\.00"
)
