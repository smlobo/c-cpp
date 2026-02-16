declare i32 @printf(ptr, ...)

declare double @testcse(double)

@fmt = private constant [15 x i8] c"result = %.2f\0A\00"

define i32 @main() {
entry:
    %res = call double (double) @testcse(double 5.000000e+00)
    call i32 (ptr, ...) @printf(ptr getelementptr (ptr, ptr @fmt), double %res)
    ret i32 0
}
