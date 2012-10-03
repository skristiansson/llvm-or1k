# RUN: llvm-mc -arch=or1k -disassemble %s | FileCheck %s

    0x15 0x00 0x00 0x01
# CHECK: l.nop 1
