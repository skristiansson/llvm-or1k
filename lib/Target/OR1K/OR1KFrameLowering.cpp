//===-- OR1KFrameLowering.cpp - OR1K Frame Information --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the OR1K implementation of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#include "OR1KFrameLowering.h"
#include "llvm/CodeGen/MachineFunction.h"

using namespace llvm;

void OR1KFrameLowering::emitPrologue(MachineFunction &MF) const {
}

void OR1KFrameLowering::emitEpilogue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {
}
