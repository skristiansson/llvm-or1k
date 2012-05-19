; RUN: llc -march=or1k < %s | FileCheck %s

define i32 @jumptable(i32 %a) {
entry:
  %retval = alloca i32, align 4
  %a.addr = alloca i32, align 4
  store i32 %a, i32* %a.addr, align 4
  %0 = load i32* %a.addr, align 4
  switch i32 %0, label %sw.default [
    i32 0, label %sw.bb
    i32 1, label %sw.bb1
    i32 2, label %sw.bb2
    i32 3, label %sw.bb3
    i32 4, label %sw.bb4
    i32 5, label %sw.bb5
    i32 6, label %sw.bb6
    i32 7, label %sw.bb7
    i32 8, label %sw.bb8
    i32 9, label %sw.bb9
    i32 10, label %sw.bb10
    i32 11, label %sw.bb11
  ]

sw.bb:                                            ; preds = %entry
  store i32 0, i32* %retval
  br label %return

sw.bb1:                                           ; preds = %entry
  store i32 1, i32* %retval
  br label %return

sw.bb2:                                           ; preds = %entry
  store i32 2, i32* %retval
  br label %return

sw.bb3:                                           ; preds = %entry
  store i32 3, i32* %retval
  br label %return

sw.bb4:                                           ; preds = %entry
  store i32 4, i32* %retval
  br label %return

sw.bb5:                                           ; preds = %entry
  store i32 5, i32* %retval
  br label %return

sw.bb6:                                           ; preds = %entry
  store i32 6, i32* %retval
  br label %return

sw.bb7:                                           ; preds = %entry
  store i32 7, i32* %retval
  br label %return

sw.bb8:                                           ; preds = %entry
  store i32 8, i32* %retval
  br label %return

sw.bb9:                                           ; preds = %entry
  store i32 9, i32* %retval
  br label %return

sw.bb10:                                          ; preds = %entry
  store i32 10, i32* %retval
  br label %return

sw.bb11:                                          ; preds = %entry
  store i32 11, i32* %retval
  br label %return

sw.default:                                       ; preds = %entry
  store i32 12, i32* %retval
  br label %return

return:                                           ; preds = %sw.default, %sw.bb11, %sw.bb10, %sw.bb9, %sw.bb8, %sw.bb7, %sw.bb6, %sw.bb5, %sw.bb4, %sw.bb3, %sw.bb2, %sw.bb1, %sw.bb
  %1 = load i32* %retval
  ret i32 %1
}
; CHECK: jumptable:
