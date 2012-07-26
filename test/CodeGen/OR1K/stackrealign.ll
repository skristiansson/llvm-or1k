; RUN: llc -march=or1k < %s | FileCheck %s

define void @f0() nounwind {
entry:
  %ll = alloca i64, align 8
  store volatile i64 1, i64* %ll, align 8
  ret void
}
; CHECK: f0:
; check that the framepointer is used
; CHECK: l.sw    -4(r1), r2
; CHECK: l.addi  r2, r1, 0
; check that the stack realigns to an 8 byte boundary
; CHECK: l.addi  [[SCRATCHREG:r[0-9]+]], r1, -16
; CHECK: l.srli  [[SCRATCHREG]], [[SCRATCHREG]], 3
; CHECK: l.slli  r1, [[SCRATCHREG]], 3
; check that the stack pointer is used to address stack object
; CHECK: l.addi  [[REG:r[0-9]+]], r0, 1
; CHECK: l.sw    4(r1), [[REG]]
; CHECK: l.movhi [[REG:r[0-9]+]], 0
; CHECK: l.sw    0(r1), [[REG]]
; check that the stack pointer is restored from the frame pointer
; CHECK: l.addi  r1, r2, 0

; Test with a dynamically allocated stack object present
define void @f1(i32 %n) nounwind {
entry:
  %ll = alloca i64, align 8
  %vla = alloca i32, i32 %n, align 4
  store volatile i64 1, i64* %ll, align 8
  store volatile i32 %n, i32* %vla, align 4
  ret void
}

; CHECK: f1:
; check that the framepointer is used
; CHECK: l.sw    -4(r1), r2
; and that the base reg is saved to the stack
; CHECK: l.sw    -8(r1), [[BASEREG:r[0-9]+]]
; CHECK: l.addi  r2, r1, 0
; check that the stack realigns to an 8 byte boundary
; CHECK: l.addi  [[SCRATCHREG:r[0-9]+]], r1, -16
; CHECK: l.srli  [[SCRATCHREG]], [[SCRATCHREG]], 3
; CHECK: l.slli  r1, [[SCRATCHREG]], 3
; check that the realigned stack pointer is copied
; to the base register and that the base register
; is used to address the local stack object
; CHECK: l.ori   [[BASEREG]], r1, 0
; CHECK: l.addi  [[REG:r[0-9]+]], r0, 1
; CHECK: l.sw    4([[BASEREG]]), [[REG]]
; CHECK: l.movhi [[REG:r[0-9]+]], 0
; CHECK: l.sw    0([[BASEREG]]), [[REG]]
; check that the stack pointer is restored from the frame pointer
; CHECK: l.addi  r1, r2, 0
