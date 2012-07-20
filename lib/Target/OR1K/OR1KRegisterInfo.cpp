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
#include "llvm/CodeGen/RegisterScavenging.h"
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
  return CSR_SaveList;
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
  return Reserved;
}

bool
OR1KRegisterInfo::requiresRegisterScavenging(const MachineFunction &MF) const {
  return true;
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
  bool HasFP = TFI->hasFP(MF);
  DebugLoc dl = MI.getDebugLoc();

  while (!MI.getOperand(i).isFI()) {
    ++i;
    assert(i < MI.getNumOperands() && "Instr doesn't have FrameIndex operand!");
  }

  int FrameIndex = MI.getOperand(i).getIndex();

  // Addressable stack objects are accessed using neg. offsets from %fp
  int Offset = MF.getFrameInfo()->getObjectOffset(FrameIndex) +
               MI.getOperand(i+1).getImm();

  if (!HasFP)
    Offset += MF.getFrameInfo()->getStackSize();

  // Replace frame index with a frame pointer reference.
  // If the offset is small enough to fit in the immediate field, directly
  // encode it.
  // Otherwise scavenge a register and encode it in to a MOVHI - ORI sequence
  if (!isInt<16>(Offset)) {
    assert(RS && "Register scavenging must be on");
    unsigned Reg = RS->FindUnusedReg(&OR1K::GPRRegClass);
    if (!Reg)
       Reg = RS->scavengeRegister(&OR1K::GPRRegClass, II, SPAdj);
    assert(Reg && "Register scavenger failed");

    // Reg = hi(offset) | lo(offset)
    BuildMI(*MI.getParent(), II, dl, TII.get(OR1K::MOVHI), Reg)
      .addImm((uint32_t)Offset >> 16);
    BuildMI(*MI.getParent(), II, dl, TII.get(OR1K::ORI), Reg)
      .addReg(Reg).addImm(Offset & 0xffffU);
    // Reg = Reg + Sp
    MI.setDesc(TII.get(OR1K::ADD));
    MI.getOperand(i).ChangeToRegister(Reg, false, false, true);
    MI.getOperand(i+1).ChangeToRegister((HasFP ? OR1K::R2 : OR1K::R1), false);

    return;
  }

  MI.getOperand(i).ChangeToRegister((HasFP ? OR1K::R2 : OR1K::R1), false);
  MI.getOperand(i+1).ChangeToImmediate(Offset);
}

void OR1KRegisterInfo::
processFunctionBeforeFrameFinalized(MachineFunction &MF) const {}

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
