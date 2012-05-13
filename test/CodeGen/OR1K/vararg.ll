; RUN: llc -march=or1k < %s | FileCheck %s

declare void @vararg(i32, ...)
define void @foo() {
entry:
  tail call void (i32, ...)* @vararg(i32 0, i32 1, i32 2)
  ret void
}

; check that variable args get passed on stack
; CHECK: l.addi r3, r0, 2
; CHECK: l.sw 4(r1), r3
; CHECK: l.addi r3, r0, 1
; CHECK: l.sw 0(r1), r3
; check that fixed arg get passed in register r3
; CHECK: l.movhi r3, 0
; CHECK-NOT: l.sw
