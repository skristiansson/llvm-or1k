//===-- OR1KInstrInfo.cpp - OR1K Instruction Information --------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the OR1K implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "OR1KInstrInfo.h"
#include "OR1K.h"
#include "OR1KMachineFunctionInfo.h"
#include "OR1KSubtarget.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallVector.h"

#define GET_INSTRINFO_CTOR
#include "OR1KGenInstrInfo.inc"

using namespace llvm;

OR1KInstrInfo::OR1KInstrInfo()
  : OR1KGenInstrInfo(OR1K::ADJCALLSTACKDOWN, OR1K::ADJCALLSTACKUP),
    RI(*this) {
}

void OR1KInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                MachineBasicBlock::iterator I, DebugLoc DL,
                                unsigned DestReg, unsigned SrcReg,
                                bool KillSrc) const {
  if (OR1K::GPRRegClass.contains(DestReg, SrcReg))
    BuildMI(MBB, I, DL, get(OR1K::ORI), DestReg)
      .addReg(SrcReg, getKillRegState(KillSrc))
      .addImm(0);
  else
    llvm_unreachable("Impossible reg-to-reg copy");
}

void OR1KInstrInfo::
storeRegToStackSlot(MachineBasicBlock &MBB, MachineBasicBlock::iterator I,
                    unsigned SrcReg, bool isKill, int FI,
                    const TargetRegisterClass *RC,
                    const TargetRegisterInfo *TRI) const {
  DebugLoc DL;
  if (I != MBB.end()) DL = I->getDebugLoc();

  if (RC == OR1K::GPRRegisterClass)
    BuildMI(MBB, I, DL, get(OR1K::SW)).addReg(SrcReg, getKillRegState(isKill))
      .addFrameIndex(FI).addImm(0);
  else
    llvm_unreachable("Can't store this register to stack slot");
}

void OR1KInstrInfo::
loadRegFromStackSlot(MachineBasicBlock &MBB, MachineBasicBlock::iterator I,
                     unsigned DestReg, int FI,
                     const TargetRegisterClass *RC,
                     const TargetRegisterInfo *TRI) const {
  DebugLoc DL;
  if (I != MBB.end()) DL = I->getDebugLoc();

  if (RC == OR1K::GPRRegisterClass)
    BuildMI(MBB, I, DL, get(OR1K::LWZ), DestReg).addFrameIndex(FI).addImm(0);
  else
    llvm_unreachable("Can't load this register from stack slot");
}
