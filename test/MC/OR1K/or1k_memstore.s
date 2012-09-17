# RUN: llvm-mc -arch=or1k -show-encoding %s | FileCheck %s

    l.sb 4(r2), r1
# CHECK: # encoding: [0xd8,0x02,0x08,0x04]

    l.sh 4(r2), r1
# CHECK: # encoding: [0xdc,0x02,0x08,0x04]

    l.sw 4(r2), r1
# CHECK: # encoding: [0xd4,0x02,0x08,0x04]
