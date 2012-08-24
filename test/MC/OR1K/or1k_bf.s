# RUN: llvm-mc -arch=or1k -show-inst %s | FileCheck %s

    l.bf 0
# CHECK: <MCInst #{{[0-9]+}} BF
# CHECK-NEXT: <MCOperand Imm:0>>

    l.bnf 0
# CHECK: <MCInst #{{[0-9]+}} BNF
# CHECK-NEXT: <MCOperand Imm:0>>

    l.cmov r1, r2, r3
# CHECK: <MCInst #{{[0-9]+}} CMOV
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>
# CHECK-NEXT: <MCOperand Reg:4>>

    l.j 0
# CHECK: <MCInst #{{[0-9]+}} J
# CHECK-NEXT: <MCOperand Imm:0>>

    l.jal 0
# CHECK: <MCInst #{{[0-9]+}} JAL
# CHECK-NEXT: <MCOperand Imm:0>>

    l.jalr r2
# CHECK: <MCInst #{{[0-9]+}} JALR
# CHECK-NEXT: <MCOperand Reg:3>>

    l.jr r2
# CHECK: <MCInst #{{[0-9]+}} JR
# CHECK-NEXT: <MCOperand Reg:3>>
