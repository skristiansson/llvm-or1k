# RUN: llvm-mc -arch=or1k -show-encoding %s | FileCheck %s

    l.nop 1
# CHECK: # encoding: [0x01,0x00,0x00,0x15]
