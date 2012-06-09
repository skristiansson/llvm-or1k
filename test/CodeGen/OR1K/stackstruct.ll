; RUN: llc -march=or1k < %s | FileCheck %s

%struct.s = type { i32, i32 }

define void @foo() nounwind {
entry:
  %s = alloca %struct.s, align 4
  %a = getelementptr inbounds %struct.s* %s, i32 0, i32 0
  store i32 1, i32* %a, align 4
  %b = getelementptr inbounds %struct.s* %s, i32 0, i32 1
  store i32 2, i32* %b, align 4
  ret void
}

; CHECK: foo:
; CHECK: l.addi  r3, r0, 1
; CHECK: l.sw    0(r1), r3
; CHECK: l.addi  r3, r0, 2
; CHECK: l.sw    4(r1), r3
