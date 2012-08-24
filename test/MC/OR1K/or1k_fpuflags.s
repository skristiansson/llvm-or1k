# RUN: llvm-mc -arch=or1k -show-inst %s | FileCheck %s

    lf.sfeq.s r1, r2
# CHECK: <MCInst #{{[0-9]+}} SFEQ
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>>

    lf.sfge.s r1, r2
# CHECK: <MCInst #{{[0-9]+}} SFGEOrrf32
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>>

    lf.sfgt.s r1, r2
# CHECK: <MCInst #{{[0-9]+}} SFGTOrrf32
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>>

    lf.sfle.s r1, r2
# CHECK: <MCInst #{{[0-9]+}} SFLEOrrf32
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>>

    lf.sflt.s r1, r2
# CHECK: <MCInst #{{[0-9]+}} SFLTOrrf32
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>>

    lf.sfne.s r1, r2
# CHECK: <MCInst #{{[0-9]+}} SFNE
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>>
