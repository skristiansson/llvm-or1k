# RUN: llvm-mc -arch=or1k -show-inst %s | FileCheck %s

    l.sb 0(r2), r1
# CHECK: <MCInst #{{[0-9]+}} SB
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>
# CHECK-NEXT: <MCOperand Imm:0>>

    l.sh 0(r2), r1
# CHECK: <MCInst #{{[0-9]+}} SH
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>
# CHECK-NEXT: <MCOperand Imm:0>>

    l.sw 0(r2), r1
# CHECK: <MCInst #{{[0-9]+}} SW
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>
# CHECK-NEXT: <MCOperand Imm:0>>
