add_test(NAME mandelbrot
        COMMAND $<TARGET_FILE:kaleidoscopec> ${CMAKE_CURRENT_SOURCE_DIR}/mandelbrot.ks
)
set_tests_properties(mandelbrot PROPERTIES
        FAIL_REGULAR_EXPRESSION "Error"
)

add_test(NAME mandelbrot-link
        COMMAND llvm-link mandelbrot.ll ${CMAKE_CURRENT_SOURCE_DIR}/test.ll -o linked.bc
)
set_tests_properties(mandelbrot-link PROPERTIES DEPENDS mandelbrot)

add_test(NAME mandelbrot-execute
        COMMAND lli linked.bc
)
set_tests_properties(mandelbrot-execute PROPERTIES
        DEPENDS mandelbrot-link
        PASS_REGULAR_EXPRESSION ""
)
