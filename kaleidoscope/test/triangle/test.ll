declare i32 @putchar(i32)

declare double @triangle(double)

define void @putchard(double %d) {
entry:
    %i = fptosi double %d to i32
    call i32 (i32) @putchar(i32 %i)
    ret void
}

define i32 @main() {
entry:
    %r1 = call double (double) @triangle(double 4.000000e+00)
    ret i32 0
}
