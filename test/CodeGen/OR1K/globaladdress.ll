; RUN: llc -march=or1k < %s | FileCheck %s

@bar = common global i32 0, align 4

define void @foo() nounwind {
entry:
  store i32 0, i32* @bar, align 4
  ret void
}

; CHECK: l.movhi r3, hi(bar)
; CHECK: l.ori r3, r3, lo(bar)
