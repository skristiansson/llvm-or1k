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
#include "llvm/Target/TargetFrameLowering.h"
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
  const TargetFrameLowering *TFI = MF.getTarget().getFrameLowering();

  Reserved.set(OR1K::R0);
  Reserved.set(OR1K::R1);
  if (TFI->hasFP(MF))
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
  // Discard ADJCALLSTACKDOWN, ADJCALLSTACKUP instructions.
  MBB.erase(I);
}


void
OR1KRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                       int SPAdj, RegScavenger *RS) const {
  assert(SPAdj == 0 && "Unexpected");

  unsigned i = 0;
  MachineInstr &MI = *II;
  MachineFunction &MF = *MI.getParent()->getParent();
  const TargetFrameLowering *TFI = MF.getTarget().getFrameLowering();
  bool hasFP = TFI->hasFP(MF);

  while (!MI.getOperand(i).isFI()) {
    ++i;
    assert(i < MI.getNumOperands() && "Instr doesn't have FrameIndex operand!");
  }

  int FrameIndex = MI.getOperand(i).getIndex();

  // Addressable stack objects are accessed using neg. offsets from %fp
  int Offset = MF.getFrameInfo()->getObjectOffset(FrameIndex) +
               MI.getOperand(i+1).getImm();

  // FIXME: implement a MOVHI - ORI sequence if imm does not fit
  // Sparc has an example
  assert(isInt<16>(Offset) && "Offset is not small enough to fit in imm field");

  // Replace frame index with a frame pointer reference.
  // If the offset is small enough to fit in the immediate field, directly
  // encode it.
  MI.getOperand(i).ChangeToRegister((hasFP ? OR1K::R2 : OR1K::R1), false);
  MI.getOperand(i+1).ChangeToImmediate(-Offset);
}

void OR1KRegisterInfo::
processFunctionBeforeFrameFinalized(MachineFunction &MF) const {}

void OR1KRegisterInfo::emitPrologue(MachineFunction &MF) const {
}

void OR1KRegisterInfo::emitEpilogue(MachineFunction &MF,
                                     MachineBasicBlock &MBB) const {
}

unsigned OR1KRegisterInfo::getRARegister() const {
  return OR1K::R9;
}

unsigned OR1KRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  const TargetFrameLowering *TFI = MF.getTarget().getFrameLowering();

  return TFI->hasFP(MF) ? OR1K::R2 : OR1K::R1;
}

unsigned OR1KRegisterInfo::getEHExceptionRegister() const {
  llvm_unreachable("What is the exception register");
  return 0;
}

unsigned OR1KRegisterInfo::getEHHandlerRegister() const {
  llvm_unreachable("What is the exception handler register");
  return 0;
}
