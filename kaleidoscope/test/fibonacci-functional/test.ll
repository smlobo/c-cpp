declare i32 @printf(ptr, ...)

declare double @fibf(double)

@fmt = private constant [36 x i8] c"fibonacci functional [%.0f] = %.0f\0A\00"

define i32 @main() {
entry:
    %r1 = call double (double) @fibf(double 3.000000e+00)
    call i32 (ptr, ...) @printf(ptr getelementptr (ptr, ptr @fmt), double 3.0, double %r1)
    %r2 = call double (double) @fibf(double 4.0)
    call i32 (ptr, ...) @printf(ptr getelementptr (ptr, ptr @fmt), double 4.0, double %r2)
    %r3 = call double (double) @fibf(double 5.0)
    call i32 (ptr, ...) @printf(ptr getelementptr (ptr, ptr @fmt), double 5.0, double %r3)
    %r4 = call double (double) @fibf(double 6.0)
    call i32 (ptr, ...) @printf(ptr getelementptr (ptr, ptr @fmt), double 6.0, double %r4)
    ret i32 0
}
