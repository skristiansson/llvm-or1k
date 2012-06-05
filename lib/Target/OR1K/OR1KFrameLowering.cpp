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

// determineFrameLayout - Determine the size of the frame and maximum call
/// frame size.
void OR1KFrameLowering::determineFrameLayout(MachineFunction &MF) const {
  MachineFrameInfo *MFI = MF.getFrameInfo();

  // Get the number of bytes to allocate from the FrameInfo.
  unsigned FrameSize = MFI->getStackSize();

  // Get the alignments provided by the target.
  unsigned TargetAlign = MF.getTarget().getFrameLowering()->getStackAlignment();
  // Get the maximum call frame size of all the calls.
  unsigned maxCallFrameSize = MFI->getMaxCallFrameSize();

  // If we have dynamic alloca then maxCallFrameSize needs to be aligned so
  // that allocations will be aligned.
  if (MFI->hasVarSizedObjects())
    maxCallFrameSize = RoundUpToAlignment(maxCallFrameSize, TargetAlign);

  // Update maximum call frame size.
  MFI->setMaxCallFrameSize(maxCallFrameSize);

  // Include call frame size in total.
  FrameSize += maxCallFrameSize;

  // Make sure the frame is aligned.
  FrameSize = RoundUpToAlignment(FrameSize, TargetAlign);

  // Update frame info.
  MFI->setStackSize(FrameSize);
}

// Iterates through each basic block in a machine function and replaces
// ADJDYNALLOC pseudo instructions with a OR1K:ADDI with the
// maximum call frame size as the immediate.
void OR1KFrameLowering::replaceAdjDynAllocPseudo(MachineFunction &MF) const {
  const OR1KInstrInfo &TII =
    *static_cast<const OR1KInstrInfo*>(MF.getTarget().getInstrInfo());
  unsigned MaxCallFrameSize = MF.getFrameInfo()->getMaxCallFrameSize();

  for (MachineFunction::iterator MBB = MF.begin(), E = MF.end();
      MBB != E; ++MBB) {
    MachineBasicBlock::iterator MBBI = MBB->begin();
    while (MBBI != MBB->end()) {
      MachineInstr *MI = MBBI++;
      if (MI->getOpcode() == OR1K::ADJDYNALLOC) {
        DebugLoc DL = MI->getDebugLoc();
        unsigned Dst = MI->getOperand(0).getReg();
        unsigned Src = MI->getOperand(1).getReg();

        BuildMI(*MBB, MI, DL, TII.get(OR1K::ADDI), Dst)
          .addReg(Src).addImm(MaxCallFrameSize);
        MI->eraseFromParent();
      }
    }
  }
}

void OR1KFrameLowering::emitPrologue(MachineFunction &MF) const {
  MachineBasicBlock &MBB   = MF.front();
  MachineFrameInfo *MFI    = MF.getFrameInfo();
  const OR1KInstrInfo &TII =
    *static_cast<const OR1KInstrInfo*>(MF.getTarget().getInstrInfo());
  MachineBasicBlock::iterator MBBI = MBB.begin();
  DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();

  // Determine the correct frame layout
  determineFrameLayout(MF);

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

  if (isInt<16>(StackSize)) {
    // Adjust stack : l.addi r1, r1, -imm
    if (StackSize) {
      BuildMI(MBB, MBBI, DL, TII.get(OR1K::ADDI), OR1K::R1)
        .addReg(OR1K::R1).addImm(-StackSize);
    }
  } else {
    // FIXME: allocate a register instead of just using r13
    BuildMI(MBB, MBBI, DL, TII.get(OR1K::MOVHI), OR1K::R13)
      .addImm((uint32_t)-StackSize >> 16);
    BuildMI(MBB, MBBI, DL, TII.get(OR1K::ORI), OR1K::R13)
      .addReg(OR1K::R13).addImm(-StackSize & 0xffffU);
    BuildMI(MBB, MBBI, DL, TII.get(OR1K::ADD), OR1K::R1)
      .addReg(OR1K::R1).addReg(OR1K::R13);
  }

  if (MFI->hasVarSizedObjects()) {
    replaceAdjDynAllocPseudo(MF);
  }
}

void OR1KFrameLowering::emitEpilogue(MachineFunction &MF,
                                    MachineBasicBlock &MBB) const {
  MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();
  MachineFrameInfo *MFI            = MF.getFrameInfo();
  const OR1KInstrInfo &TII =
    *static_cast<const OR1KInstrInfo*>(MF.getTarget().getInstrInfo());

  DebugLoc dl = MBBI->getDebugLoc();

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
    assert(isInt<16>(StackSize) && "Offset to large for imm");
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
