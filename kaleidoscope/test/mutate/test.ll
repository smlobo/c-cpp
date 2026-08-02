declare i32 @printf(ptr, ...)

declare double @testmutate()

@fmt = private constant [6 x i8] c"%.2f\0A\00"

define void @printd(double %d) {
    call i32 (ptr, ...) @printf(ptr getelementptr (ptr, ptr @fmt), double %d)
    ret void
}

define i32 @main() {
entry:
    call double (double) @testmutate(double 123.000000e+00)
    ret i32 0
}
