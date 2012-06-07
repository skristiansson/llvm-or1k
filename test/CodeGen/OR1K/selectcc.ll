; RUN: llc -march=or1k < %s | FileCheck %s

define i32 @eqrr(i32 %a, i32 %b) {
entry:
  %cmp = icmp eq i32 %a, %b
  %. = sext i1 %cmp to i32
  ret i32 %.
}
; CHECK: eqrr:
; CHECK: l.sfeq r3, r4
; CHECK: l.bf

define i32 @eqri(i32 %a) {
entry:
  %cmp = icmp eq i32 %a, 170
  %. = sext i1 %cmp to i32
  ret i32 %.
}
; CHECK: eqri:
; CHECK: l.sfeqi r3, 170
; CHECK: l.bf

define i32 @sgtrr(i32 %a, i32 %b) {
entry:
  %cmp = icmp sgt i32 %a, %b
  %. = sext i1 %cmp to i32
  ret i32 %.
}
; CHECK: sgtrr:
; CHECK: l.sfgts r3, r4
; CHECK: l.bf

define i32 @sgtri(i32 %a) {
entry:
  %cmp = icmp sgt i32 %a, 170
  %. = sext i1 %cmp to i32
  ret i32 %.
}
; CHECK: sgtri:
; CHECK: l.sfgtsi r3, 170
; CHECK: l.bf

define i32 @sgerr(i32 %a, i32 %b) {
entry:
  %cmp = icmp sge i32 %a, %b
  %. = sext i1 %cmp to i32
  ret i32 %.
}
; CHECK: sgerr:
; CHECK: l.sfges r3, r4
; CHECK: l.bf

define i32 @sgeri(i32 %a) {
entry:
  %cmp = icmp sge i32 %a, 170
  %. = sext i1 %cmp to i32
  ret i32 %.
}

define i32 @sltrr(i32 %a, i32 %b) {
entry:
  %cmp = icmp slt i32 %a, %b
  %. = sext i1 %cmp to i32
  ret i32 %.
}
; CHECK: sltrr:
; CHECK: l.sflts r3, r4
; CHECK: l.bf

define i32 @sltri(i32 %a) {
entry:
  %cmp = icmp slt i32 %a, 170
  %. = sext i1 %cmp to i32
  ret i32 %.
}
; CHECK: sltri:
; CHECK: l.sfltsi r3, 170
; CHECK: l.bf

define i32 @slerr(i32 %a, i32 %b) {
entry:
  %cmp = icmp sle i32 %a, %b
  %. = sext i1 %cmp to i32
  ret i32 %.
}
; CHECK: slerr:
; CHECK: l.sfles r3, r4
; CHECK: l.bf

define i32 @sleri(i32 %a) {
entry:
  %cmp = icmp sle i32 %a, 170
  %. = sext i1 %cmp to i32
  ret i32 %.
}

define i32 @nerr(i32 %a, i32 %b) {
entry:
  %cmp = icmp ne i32 %a, %b
  %. = sext i1 %cmp to i32
  ret i32 %.
}
; CHECK: nerr:
; CHECK: l.sfne r3, r4
; CHECK: l.bf

define i32 @neri(i32 %a) {
entry:
  %cmp = icmp ne i32 %a, 170
  %. = sext i1 %cmp to i32
  ret i32 %.
}
; CHECK: neri:
; CHECK: l.sfnei r3, 170
; CHECK: l.bf

define i32 @ugtrr(i32 %a, i32 %b) {
entry:
  %cmp = icmp ugt i32 %a, %b
  %. = sext i1 %cmp to i32
  ret i32 %.
}
; CHECK: ugtrr:
; CHECK: l.sfgtu r3, r4
; CHECK: l.bf

define i32 @ugtri(i32 %a) {
entry:
  %cmp = icmp ugt i32 %a, 170
  %. = sext i1 %cmp to i32
  ret i32 %.
}
; CHECK: ugtri:
; CHECK: l.sfgtui r3, 170
; CHECK: l.bf

define i32 @ugerr(i32 %a, i32 %b) {
entry:
  %cmp = icmp uge i32 %a, %b
  %. = sext i1 %cmp to i32
  ret i32 %.
}
; CHECK: ugerr:
; CHECK: l.sfgeu r3, r4
; CHECK: l.bf

define i32 @ugeri(i32 %a) {
entry:
  %cmp = icmp uge i32 %a, 170
  %. = sext i1 %cmp to i32
  ret i32 %.
}

define i32 @ultrr(i32 %a, i32 %b) {
entry:
  %cmp = icmp ult i32 %a, %b
  %. = sext i1 %cmp to i32
  ret i32 %.
}
; CHECK: ultrr:
; CHECK: l.sfltu r3, r4
; CHECK: l.bf

define i32 @ultri(i32 %a) {
entry:
  %cmp = icmp ult i32 %a, 170
  %. = sext i1 %cmp to i32
  ret i32 %.
}
; CHECK: ultri:
; CHECK: l.sfltui r3, 170
; CHECK: l.bf

define i32 @ulerr(i32 %a, i32 %b) {
entry:
  %cmp = icmp ule i32 %a, %b
  %. = sext i1 %cmp to i32
  ret i32 %.
}
; CHECK: ulerr:
; CHECK: l.sfleu r3, r4
; CHECK: l.bf

define i32 @uleri(i32 %a) {
entry:
  %cmp = icmp ule i32 %a, 170
  %. = sext i1 %cmp to i32
  ret i32 %.
}

define i32 @ugtri2(i32 %a) {
entry:
  %cmp = icmp ugt i32 %a, 43520
  %. = sext i1 %cmp to i32
  ret i32 %.
}

; CHECK: ugtri2:
; CHECK: l.ori   r4, r0, 43520
; CHECK: l.sfgtu r3, r4
; CHECK: l.bf
