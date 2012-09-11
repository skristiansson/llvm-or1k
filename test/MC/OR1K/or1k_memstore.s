# RUN: llvm-mc -arch=or1k -show-encoding %s | FileCheck %s

    l.sb 4(r2), r1
# CHECK: # encoding: [0x04,0x08,0x02,0xd8]

    l.sh 4(r2), r1
# CHECK: # encoding: [0x04,0x08,0x02,0xdc]

    l.sw 4(r2), r1
# CHECK: # encoding: [0x04,0x08,0x02,0xd4]
