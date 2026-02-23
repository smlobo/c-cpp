declare i32 @putchar(i32)

declare double @foo()
declare double @mandel(double %realstart, double %imagstart, double %realmag, double %imagmag)

define void @putchard(double %d) {
entry:
    %i = fptosi double %d to i32
    call i32 (i32) @putchar(i32 %i)
    ret void
}

define i32 @main() {
entry:
    call double @foo()
    call double @mandel(double -2.3, double -1.3, double 0.05, double 0.07);
    call double @mandel(double -2.0, double -1.0, double 0.02, double 0.04);
    ret i32 0
}
