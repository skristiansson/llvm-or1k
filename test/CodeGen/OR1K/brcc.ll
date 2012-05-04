; RUN: llc -march=or1k < %s | FileCheck %s

define void @brcc(i32 %a) nounwind {
entry:
  %tobool1 = icmp eq i32 %a, 0
  br i1 %tobool1, label %while.end, label %while.body
while.body:                                       ; preds = %entry, %while.body
  %a.addr.02 = phi i32 [ %dec, %while.body ], [ %a, %entry ]
  %dec = add nsw i32 %a.addr.02, -1
  %tobool = icmp eq i32 %dec, 0
  br i1 %tobool, label %while.end, label %while.body
while.end:                                        ; preds = %while.body, %entry
  ret void
}
; CHECK: brcc:
; CHECK: l.sfnei r3, 0
; CHECK: l.bf .L
; CHECK-NOT: l.j .L
