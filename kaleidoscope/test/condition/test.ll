declare i32 @printf(ptr, ...)

declare double @testif(double)

@fmt = private constant [22 x i8] c"result: %.2f -> %.2f\0A\00"

define i32 @main() {
entry:
    %n1 = fadd double 3.000000e+00, 0.000000e+00
    %r1 = call double (double) @testif(double %n1)
    call i32 (ptr, ...) @printf(ptr getelementptr (ptr, ptr @fmt), double %n1, double %r1)

    %n2 = fadd double -3.000000e+00, 0.000000e+00
    %r2 = call double (double) @testif(double %n2)
    call i32 (ptr, ...) @printf(ptr getelementptr (ptr, ptr @fmt), double %n2, double %r2)

    ret i32 0
}
