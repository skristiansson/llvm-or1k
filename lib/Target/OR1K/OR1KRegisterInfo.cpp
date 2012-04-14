//===-- OR1KRegisterInfo.cpp - OR1K Register Information --------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the OR1K implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "OR1K.h"
#include "OR1KRegisterInfo.h"
#include "OR1KSubtarget.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineLocation.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetInstrInfo.h"
#include "llvm/Type.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/STLExtras.h"
#define GET_REGINFO_TARGET_DESC
#include "OR1KGenRegisterInfo.inc"
using namespace llvm;

OR1KRegisterInfo::OR1KRegisterInfo(const TargetInstrInfo &tii)
  : OR1KGenRegisterInfo(OR1K::R9), TII(tii) {
}

const uint16_t*
OR1KRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  static const uint16_t CalleeSavedRegs[] = {
    OR1K::R14, OR1K::R16, OR1K::R18,
    OR1K::R9, OR1K::R14, OR1K::R16, OR1K::R18,
    OR1K::R20, OR1K::R22, OR1K::R24, OR1K::R26,
    OR1K::R28, OR1K::R30
  };
  return CalleeSavedRegs;
}

BitVector OR1KRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  Reserved.set(OR1K::R0);
  Reserved.set(OR1K::R1);
  Reserved.set(OR1K::R2);
  Reserved.set(OR1K::R9);
  Reserved.set(OR1K::R10);
  Reserved.set(OR1K::R11);
  return Reserved;
}

bool OR1KRegisterInfo::hasFP(const MachineFunction &MF) const {
  return false;
}


void OR1KRegisterInfo::
eliminateCallFramePseudoInstr(MachineFunction &MF, MachineBasicBlock &MBB,
                              MachineBasicBlock::iterator I) const {
}


void
OR1KRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                       int SPAdj, RegScavenger *RS) const {
}

void OR1KRegisterInfo::
processFunctionBeforeFrameFinalized(MachineFunction &MF) const {}

void OR1KRegisterInfo::emitPrologue(MachineFunction &MF) const {
}

void OR1KRegisterInfo::emitEpilogue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {
}

unsigned OR1KRegisterInfo::getRARegister() const {
  return 0;
}

unsigned OR1KRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return 0;
}

unsigned OR1KRegisterInfo::getEHExceptionRegister() const {
  llvm_unreachable("What is the exception register");
  return 0;
}

unsigned OR1KRegisterInfo::getEHHandlerRegister() const {
  llvm_unreachable("What is the exception handler register");
  return 0;
}
