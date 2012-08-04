; RUN: llc -march=or1k < %s | FileCheck %s

define float @foeq(float %a, float %b) {
entry:
  %cmp = fcmp oeq float %a, %b
  %a.b = select i1 %cmp, float %a, float %b
  ret float %a.b
}
; CHECK: foeq:
; CHECK: lf.sfeq.s
; CHECK: l.bf

define float @fogt(float %a, float %b) {
entry:
  %cmp = fcmp ogt float %a, %b
  %a.b = select i1 %cmp, float %a, float %b
  ret float %a.b
}
; CHECK: fogt:
; CHECK: lf.sfgt.s
; CHECK: l.bf

define float @foge(float %a, float %b) {
entry:
  %cmp = fcmp oge float %a, %b
  %a.b = select i1 %cmp, float %a, float %b
  ret float %a.b
}
; CHECK: foge:
; CHECK: lf.sfge.s
; CHECK: l.bf

define float @folt(float %a, float %b) {
entry:
  %cmp = fcmp olt float %a, %b
  %a.b = select i1 %cmp, float %a, float %b
  ret float %a.b
}
; CHECK: folt:
; CHECK: lf.sflt.s
; CHECK: l.bf

define float @fole(float %a, float %b) {
entry:
  %cmp = fcmp ole float %a, %b
  %a.b = select i1 %cmp, float %a, float %b
  ret float %a.b
}
; CHECK: fole:
; CHECK: lf.sfle.s
; CHECK: l.bf

define float @fone(float %a, float %b) {
entry:
  %cmp = fcmp one float %a, %b
  %a.b = select i1 %cmp, float %a, float %b
  ret float %a.b
}
; CHECK: fone:
; CHECK: lf.sfne.s
; CHECK: l.bf

define float @ford(float %a, float %b) {
entry:
  %cmp = fcmp ord float %a, %b
  %a.b = select i1 %cmp, float %a, float %b
  ret float %a.b
}
; CHECK: ford:
; CHECK: lf.sfeq.s
; CHECK: l.bf
; CHECK: lf.sfeq.s
; CHECK: l.bf

define float @fueq(float %a, float %b) {
entry:
  %cmp = fcmp ueq float %a, %b
  %a.b = select i1 %cmp, float %a, float %b
  ret float %a.b
}
; CHECK: fueq:
; CHECK: lf.sfne.s
; CHECK: l.bf

define float @fugt(float %a, float %b) {
entry:
  %cmp = fcmp ugt float %a, %b
  %a.b = select i1 %cmp, float %a, float %b
  ret float %a.b
}
; CHECK: fugt:
; CHECK: lf.sfle.s
; CHECK: l.bf

define float @fuge(float %a, float %b) {
entry:
  %cmp = fcmp uge float %a, %b
  %a.b = select i1 %cmp, float %a, float %b
  ret float %a.b
}
; CHECK: fuge:
; CHECK: lf.sflt.s
; CHECK: l.bf

define float @fult(float %a, float %b) {
entry:
  %cmp = fcmp ult float %a, %b
  %a.b = select i1 %cmp, float %a, float %b
  ret float %a.b
}
; CHECK: fult:
; CHECK: lf.sfge.s
; CHECK: l.bf

define float @fule(float %a, float %b) {
entry:
  %cmp = fcmp ule float %a, %b
  %a.b = select i1 %cmp, float %a, float %b
  ret float %a.b
}
; CHECK: fule:
; CHECK: lf.sfgt.s
; CHECK: l.bf

define float @fune(float %a, float %b) {
entry:
  %cmp = fcmp une float %a, %b
  %a.b = select i1 %cmp, float %a, float %b
  ret float %a.b
}
; CHECK: fune:
; CHECK: lf.sfeq.s
; CHECK: l.bf

define float @funo(float %a, float %b) {
entry:
  %cmp = fcmp uno float %a, %b
  %a.b = select i1 %cmp, float %a, float %b
  ret float %a.b
}
; CHECK: funo:
; CHECK: lf.sfeq.s
; CHECK: l.bf
; CHECK: lf.sfeq.s
; CHECK: l.bf
