declare i32 @printf(ptr, ...)
declare i32 @putchar(i32)

declare double @printstar(double)

@fmt = private constant [2 x i8] c"\0A\00"

define void @putchard(double %d) {
entry:
    %i = fptosi double %d to i32
    call i32 (i32) @putchar(i32 %i)
    ret void
}

define i32 @main() {
entry:
    %r1 = call double (double) @printstar(double 4.000000e+00)
    call i32 (ptr, ...) @printf(ptr getelementptr (ptr, ptr @fmt))
    ret i32 0
}
