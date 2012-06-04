; RUN: llc -march=or1k < %s | FileCheck %s

define void @foo(i32 %n) nounwind {
entry:
  %vla = alloca i32, i32 %n, align 4
  store volatile i32 0, i32* %vla, align 4
  call void @bar(i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6) nounwind
  ret void
}
; CHECK: foo:
; check that room is made on the stack for the variable sized object
; CHECK: l.slli  r3, r3, 2
; CHECK: l.sub   r3, r1, r3
; CHECK: l.ori   r1, r3, 0
; r3 is our frame index for the dynamic stack allocation,
; check that it is adjusted so there is room for the argument
; that is passed on the stack to bar()
; CHECK: l.addi  r3, r3, 4

declare void @bar(i32, i32, i32, i32, i32, i32, i32)
