; RUN: llc -march=or1k < %s | FileCheck %s

define float @f0(float %a, float %b) {
entry:
  %c = fadd float %a, %b
  ret float %c
}
; CHECK: f0:
; CHECK: lf.add.s

define float @f1(float %a, float %b) {
entry:
  %c = fsub float %a, %b
  ret float %c
}
; CHECK: f1:
; CHECK: lf.sub.s

define float @f2(float %a, float %b) {
entry:
  %c = fmul float %a, %b
  ret float %c
}
; CHECK: f2:
; CHECK: lf.mul.s

define float @f3(float %a, float %b) {
entry:
  %c = fdiv float %a, %b
  ret float %c
}
; CHECK: f3:
; CHECK: lf.div.s

define float @f4(float %a, float %b) {
entry:
  %c = frem float %a, %b
  ret float %c
}
; CHECK: f4:
; CHECK: lf.rem.s
