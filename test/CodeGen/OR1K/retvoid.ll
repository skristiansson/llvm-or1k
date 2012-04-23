; RUN: llc -march=or1k < %s | FileCheck %s

define void @foo() {
entry:
; CHECK: foo:
; CHECK: l.jr	r9
; CHECK: l.nop
  ret void
}
