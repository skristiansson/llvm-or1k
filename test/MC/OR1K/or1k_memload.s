# RUN: llvm-mc -arch=or1k -show-inst %s | FileCheck %s

    l.lbs r1, 0(r2)
# CHECK: <MCInst #{{[0-9]+}} LBS
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>
# CHECK-NEXT: <MCOperand Imm:0>>

    l.lbz r1, 0(r2)
# CHECK: <MCInst #{{[0-9]+}} LBZ
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>
# CHECK-NEXT: <MCOperand Imm:0>>

    l.lhs r1, 0(r2)
# CHECK: <MCInst #{{[0-9]+}} LHS
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>
# CHECK-NEXT: <MCOperand Imm:0>>

    l.lhz r1, 0(r2)
# CHECK: <MCInst #{{[0-9]+}} LHZ
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>
# CHECK-NEXT: <MCOperand Imm:0>>

    l.lwz r1, 0(r2)
# CHECK: <MCInst #{{[0-9]+}} LWZ
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>
# CHECK-NEXT: <MCOperand Imm:0>>