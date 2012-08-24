# RUN: llvm-mc -arch=or1k -show-inst %s | FileCheck %s

    lf.add.s r1, r2, r3
# CHECK: <MCInst #{{[0-9]+}} ADDrrf32
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>
# CHECK-NEXT: <MCOperand Reg:4>>

    lf.div.s r1, r2, r3
# CHECK: <MCInst #{{[0-9]+}} DIVrrf32
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>
# CHECK-NEXT: <MCOperand Reg:4>>

    lf.ftoi.s r1, r2
# CHECK: <MCInst #{{[0-9]+}} FTOIf32
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>>

    lf.itof.s r1, r2
# CHECK: <MCInst #{{[0-9]+}} ITOFf32
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>>

    lf.mul.s r1, r2, r3
# CHECK: <MCInst #{{[0-9]+}} MULrrf32
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>
# CHECK-NEXT: <MCOperand Reg:4>>

    lf.rem.s r1, r2, r3
# CHECK: <MCInst #{{[0-9]+}} REMrrf32
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>
# CHECK-NEXT: <MCOperand Reg:4>>

    lf.sub.s r1, r2, r3
# CHECK: <MCInst #{{[0-9]+}} SUBrrf32
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>
# CHECK-NEXT: <MCOperand Reg:4>>