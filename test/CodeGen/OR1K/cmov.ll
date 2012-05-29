; RUN: llc -march=or1k -mattr=cmov < %s | FileCheck %s

define i32 @foo(i32 %a, i32 %b) nounwind readnone {
entry:
  %cmp = icmp sgt i32 %a, %b
  %a.b = select i1 %cmp, i32 %a, i32 %b
  ret i32 %a.b
}

; CHECK: l.cmov r11, r3, r4
