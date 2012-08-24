# RUN: llvm-mc -arch=or1k -show-inst %s | FileCheck %s

    l.add r1, r2, r3
# CHECK: <MCInst #{{[0-9]+}} ADD
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>
# CHECK-NEXT: <MCOperand Reg:4>>

    l.addi r3, r4, 0
# CHECK: <MCInst #{{[0-9]+}} ADDI
# CHECK-NEXT: <MCOperand Reg:4>
# CHECK-NEXT: <MCOperand Reg:5>
# CHECK-NEXT: <MCOperand Imm:0>>

    l.and r1, r2, r3
# CHECK: <MCInst #{{[0-9]+}} AND
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>
# CHECK-NEXT: <MCOperand Reg:4>>

    l.andi r3, r4, 0
# CHECK: <MCInst #{{[0-9]+}} ANDI
# CHECK-NEXT: <MCOperand Reg:4>
# CHECK-NEXT: <MCOperand Reg:5>
# CHECK-NEXT: <MCOperand Imm:0>>

    l.div r1, r2, r3
# CHECK: <MCInst #{{[0-9]+}} DIV
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>
# CHECK-NEXT: <MCOperand Reg:4>>

    l.divu r1, r2, r3
# CHECK: <MCInst #{{[0-9]+}} DIVU
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>
# CHECK-NEXT: <MCOperand Reg:4>>

    l.movhi r1, 2
# CHECK: <MCInst #{{[0-9]+}} MOVHI
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Imm:2>>

    l.mul r1, r2, r3
# CHECK: <MCInst #{{[0-9]+}} MUL
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>
# CHECK-NEXT: <MCOperand Reg:4>>

    l.muli r3, r4, 0
# CHECK: <MCInst #{{[0-9]+}} MULI
# CHECK-NEXT: <MCOperand Reg:4>
# CHECK-NEXT: <MCOperand Reg:5>
# CHECK-NEXT: <MCOperand Imm:0>>

    l.mulu r3, r4, r5
# CHECK: <MCInst #{{[0-9]+}} MULU
# CHECK-NEXT: <MCOperand Reg:4>
# CHECK-NEXT: <MCOperand Reg:5>
# CHECK-NEXT: <MCOperand Reg:6>>

    l.or r1, r2, r3
# CHECK: <MCInst #{{[0-9]+}} OR
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>
# CHECK-NEXT: <MCOperand Reg:4>>

    l.ori r3, r4, 0
# CHECK: <MCInst #{{[0-9]+}} ORI
# CHECK-NEXT: <MCOperand Reg:4>
# CHECK-NEXT: <MCOperand Reg:5>
# CHECK-NEXT: <MCOperand Imm:0>>

    l.ror r1, r2, r3
# CHECK: <MCInst #{{[0-9]+}} ROR
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>
# CHECK-NEXT: <MCOperand Reg:4>>

    l.rori r3, r4, 0
# CHECK: <MCInst #{{[0-9]+}} ROR_ri
# CHECK-NEXT: <MCOperand Reg:4>
# CHECK-NEXT: <MCOperand Reg:5>
# CHECK-NEXT: <MCOperand Imm:0>>

    l.sll r1, r2, r3
# CHECK: <MCInst #{{[0-9]+}} SLL
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>
# CHECK-NEXT: <MCOperand Reg:4>>

    l.slli r3, r4, 0
# CHECK: <MCInst #{{[0-9]+}} SLL_ri
# CHECK-NEXT: <MCOperand Reg:4>
# CHECK-NEXT: <MCOperand Reg:5>
# CHECK-NEXT: <MCOperand Imm:0>>

    l.sra r1, r2, r3
# CHECK: <MCInst #{{[0-9]+}} SRA
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>
# CHECK-NEXT: <MCOperand Reg:4>>

    l.srai r3, r4, 0
# CHECK: <MCInst #{{[0-9]+}} SRA_ri
# CHECK-NEXT: <MCOperand Reg:4>
# CHECK-NEXT: <MCOperand Reg:5>
# CHECK-NEXT: <MCOperand Imm:0>>

    l.srl r1, r2, r3
# CHECK: <MCInst #{{[0-9]+}} SRL
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>
# CHECK-NEXT: <MCOperand Reg:4>>

    l.srli r3, r4, 0
# CHECK: <MCInst #{{[0-9]+}} SRL_ri
# CHECK-NEXT: <MCOperand Reg:4>
# CHECK-NEXT: <MCOperand Reg:5>
# CHECK-NEXT: <MCOperand Imm:0>>

    l.sub r1, r2, r3
# CHECK: <MCInst #{{[0-9]+}} SUB
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>
# CHECK-NEXT: <MCOperand Reg:4>>

    l.xor r1, r2, r3
# CHECK: <MCInst #{{[0-9]+}} XOR
# CHECK-NEXT: <MCOperand Reg:2>
# CHECK-NEXT: <MCOperand Reg:3>
# CHECK-NEXT: <MCOperand Reg:4>>

    l.xori r3, r4, 0
# CHECK: <MCInst #{{[0-9]+}} XORI
# CHECK-NEXT: <MCOperand Reg:4>
# CHECK-NEXT: <MCOperand Reg:5>
# CHECK-NEXT: <MCOperand Imm:0>>
