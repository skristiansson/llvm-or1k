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
#include "OR1KInstrInfo.h"
#include "OR1KMachineFunctionInfo.h"
#include "llvm/Function.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"

using namespace llvm;

bool OR1KFrameLowering::hasFP(const MachineFunction &MF) const {
  const MachineFrameInfo *MFI = MF.getFrameInfo();

  return (MF.getTarget().Options.DisableFramePointerElim(MF) ||
          MF.getFrameInfo()->hasVarSizedObjects() ||
          MFI->isFrameAddressTaken());
}

void OR1KFrameLowering::emitPrologue(MachineFunction &MF) const {
  MachineBasicBlock &MBB   = MF.front();
  MachineFrameInfo *MFI    = MF.getFrameInfo();
  const OR1KInstrInfo &TII =
    *static_cast<const OR1KInstrInfo*>(MF.getTarget().getInstrInfo());
  OR1KMachineFunctionInfo *OR1KFI = MF.getInfo<OR1KMachineFunctionInfo>();
  MachineBasicBlock::iterator MBBI = MBB.begin();
  DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();

  llvm::CallingConv::ID CallConv = MF.getFunction()->getCallingConv();

#if 0
  // Determine the correct frame layout
  determineFrameLayout(MF);
#endif

  // Get the number of bytes to allocate from the FrameInfo.
  unsigned StackSize = MFI->getStackSize();

  // No need to allocate space on the stack.
  if (StackSize == 0 && !MFI->adjustsStack()) return;

  int Offset = -4;

  // l.sw  stack_lock(r1), r9
  if (MFI->adjustsStack()) {
    BuildMI(MBB, MBBI, DL, TII.get(OR1K::SW))
        .addReg(OR1K::R9).addReg(OR1K::R1).addImm(Offset);
    Offset -= 4;
  }

  if (hasFP(MF)) {
    // Save frame pointer onto stack
    // l.sw  stack_loc(r1), r2
    BuildMI(MBB, MBBI, DL, TII.get(OR1K::SW))
      .addReg(OR1K::R2).addReg(OR1K::R1).addImm(Offset);

    // Set frame pointer to stack pointer
    // l.addi r2, r1, 0
    BuildMI(MBB, MBBI, DL, TII.get(OR1K::ADDI), OR1K::R2)
      .addReg(OR1K::R1).addImm(0);
  }

  // Adjust stack : l.addi r1, r1, -imm
  if (StackSize) {
    BuildMI(MBB, MBBI, DL, TII.get(OR1K::ADDI), OR1K::R1)
      .addReg(OR1K::R1).addImm(-StackSize);
  }
}

void OR1KFrameLowering::emitEpilogue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {
  MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();
  MachineFrameInfo *MFI            = MF.getFrameInfo();
  OR1KMachineFunctionInfo *OR1KFI  = MF.getInfo<OR1KMachineFunctionInfo>();
  const OR1KInstrInfo &TII =
    *static_cast<const OR1KInstrInfo*>(MF.getTarget().getInstrInfo());

  DebugLoc dl = MBBI->getDebugLoc();

  llvm::CallingConv::ID CallConv = MF.getFunction()->getCallingConv();

  int FPOffset = MFI->adjustsStack() ? -8 : -4;
  int RAOffset = -4;

  // Get the number of bytes from FrameInfo
  int StackSize = (int) MFI->getStackSize();

  if (hasFP(MF)) {
    // Set stack pointer to frame pointer
    // l.addi r1, r2, 0
    BuildMI(MBB, MBBI, dl, TII.get(OR1K::ADDI), OR1K::R1)
      .addReg(OR1K::R2).addImm(0);

    // Load frame pointer from stack
    // l.lwz  r2, stack_loc(r1)
    BuildMI(MBB, MBBI, dl, TII.get(OR1K::LWZ), OR1K::R2)
      .addReg(OR1K::R1).addImm(FPOffset);
  } else {
    // l.addi r1, r1, imm
    if (StackSize) {
      BuildMI(MBB, MBBI, dl, TII.get(OR1K::ADDI), OR1K::R1)
        .addReg(OR1K::R1).addImm(StackSize);
    }
  }

  // l.lwz r9, stack_loc(r1)
  if (MFI->adjustsStack()) {
    BuildMI(MBB, MBBI, dl, TII.get(OR1K::LWZ), OR1K::R9)
      .addReg(OR1K::R1).addImm(RAOffset);
  }
}

void OR1KFrameLowering::
processFunctionBeforeCalleeSavedScan(MachineFunction &MF,
                                     RegScavenger *RS) const {
  MachineFrameInfo *MFI = MF.getFrameInfo();
  MachineRegisterInfo& MRI = MF.getRegInfo();
  int Offset = -4;

  if (MFI->adjustsStack()) {
    MFI->CreateFixedObject(4, Offset, true);
    // Mark unused since we will save it manually in the prologue
    MRI.setPhysRegUnused(OR1K::R9);
    Offset -= 4;
  }

  if (hasFP(MF))
    MFI->CreateFixedObject(4, Offset, true);
}
