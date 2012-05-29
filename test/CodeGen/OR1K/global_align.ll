; RUN: llc -march=or1k < %s | FileCheck %s

@i8global_arr = global [4 x i8] c"\00\01\02\03", align 1
; CHECK: i8global_arr:
@i16global_arr = global [4 x i16] [i16 0, i16 1, i16 2, i16 3], align 2
; CHECK: .align 2
; CHECK: i16global_arr:
@i32global_arr = global [4 x i32] [i32 0, i32 1, i32 2, i32 3], align 4
; CHECK: .align 4
; CHECK: i32global_arr:
