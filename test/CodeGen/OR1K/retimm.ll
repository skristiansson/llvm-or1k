; RUN: llc -march=or1k < %s | FileCheck %s

define i32 @ret0() {
entry:
  ret i32 0
}

; CHECK: ret0:
; CHECK: l.movhi r11, 0
; CHECK: l.jr    r9
; CHECK: l.nop
