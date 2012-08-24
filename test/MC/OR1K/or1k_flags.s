# RUN: llvm-mc -arch=or1k -show-inst %s | FileCheck %s

    l.sfeq r1, r2
# CHECK: <MCInst #{{[0-9]+}} SFEQ
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>>

    l.sfeqi r1, 3
# CHECK: <MCInst #{{[0-9]+}} SFEQ_ri
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Imm:3>>

    l.sfges r1, r2
# CHECK: <MCInst #{{[0-9]+}} SFGES
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>>

    l.sfgesi r1, 3
# CHECK: <MCInst #{{[0-9]+}} SFGES_ri
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Imm:3>>

    l.sfgeu r1, r2
# CHECK: <MCInst #{{[0-9]+}} SFGEU
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>>

    l.sfgeui r1, 3
# CHECK: <MCInst #{{[0-9]+}} SFGEU_ri
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Imm:3>>

    l.sfgts r1, r2
# CHECK: <MCInst #{{[0-9]+}} SFGTS_rr
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>>

    l.sfgtsi r1, 3
# CHECK: <MCInst #{{[0-9]+}} SFGTS_ri
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Imm:3>>

    l.sfgtu r1, r2
# CHECK: <MCInst #{{[0-9]+}} SFGTU_rr
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>>

    l.sfgtui r1, 3
# CHECK: <MCInst #{{[0-9]+}} SFGTU_ri
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Imm:3>>

    l.sfles r1, r2
# CHECK: <MCInst #{{[0-9]+}} SFLES
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>>

    l.sflesi r1, 3
# CHECK: <MCInst #{{[0-9]+}} SFLES_ri
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Imm:3>>

    l.sfleu r1, r2
# CHECK: <MCInst #{{[0-9]+}} SFLEU
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>>

    l.sfleui r1, 3
# CHECK: <MCInst #{{[0-9]+}} SFLEU_ri
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Imm:3>>

    l.sflts r1, r2
# CHECK: <MCInst #{{[0-9]+}} SFLTS_rr
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>>

    l.sfltsi r1, 3
# CHECK: <MCInst #{{[0-9]+}} SFLTS_ri
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Imm:3>>

    l.sfltu r1, r2
# CHECK: <MCInst #{{[0-9]+}} SFLTU_rr
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>>

    l.sfltui r1, 3
# CHECK: <MCInst #{{[0-9]+}} SFLTU_ri
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Imm:3>>

    l.sfne r1, r2
# CHECK: <MCInst #{{[0-9]+}} SFNE
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>>

    l.sfnei r1, 3
# CHECK: <MCInst #{{[0-9]+}} SFNE_ri
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Imm:3>>