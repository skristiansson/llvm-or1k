# RUN: llvm-mc -arch=or1k -show-inst %s | FileCheck %s

    l.nop 1
# CHECK: <MCInst #{{[0-9]+}} NOP
# CHECK-NEXT: <MCOperand Imm:1>