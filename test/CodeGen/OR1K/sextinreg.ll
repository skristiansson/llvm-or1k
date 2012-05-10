; RUN: llc -march=or1k < %s | FileCheck %s

define signext i8 @sextinregi8(i8 signext %a, i8 signext %b) {
entry:
  %inc = add i8 %a, 1
  %dec = add i8 %b, -1
  %cmp = icmp slt i8 %inc, %dec
  %inc.dec = select i1 %cmp, i8 %inc, i8 %dec
  ret i8 %inc.dec
}
; CHECK: sextinregi8:
; CHECK: l.slli
; CHECK: l.srai

define signext i16 @sextinregi16(i16 signext %a, i16 signext %b) {
entry:
  %inc = add i16 %a, 1
  %dec = add i16 %b, -1
  %cmp = icmp slt i16 %inc, %dec
  %inc.dec = select i1 %cmp, i16 %inc, i16 %dec
  ret i16 %inc.dec
}
; CHECK: sextinregi16:
; CHECK: l.slli
; CHECK: l.srai
