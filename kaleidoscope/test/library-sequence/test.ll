declare i32 @printf(ptr, ...)
declare double @foo()

@fmt = private constant [6 x i8] c"%.2f\0A\00"

define void @printd(double %d) {
entry:
    call i32 (ptr, ...) @printf(ptr getelementptr (ptr, ptr @fmt), double %d)
    ret void
}

define i32 @main() {
entry:
    call double @foo()
    ret i32 0
}
