add_test(NAME condition
        COMMAND $<TARGET_FILE:kaleidoscopec> ${CMAKE_CURRENT_SOURCE_DIR}/condition.ks
)

add_test(NAME condition-link
        COMMAND llvm-link condition.ll ${CMAKE_CURRENT_SOURCE_DIR}/test.ll -o linked.bc
)
set_tests_properties(condition-link PROPERTIES DEPENDS condition)

add_test(NAME condition-execute
        COMMAND lli linked.bc
)
set_tests_properties(condition-execute PROPERTIES DEPENDS condition-link)
