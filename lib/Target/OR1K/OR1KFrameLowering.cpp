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
  bool requiresRA = false;//CallConv == llvm::CallingConv::MBLAZE_INTR;

#if 0
  // Determine the correct frame layout
  determineFrameLayout(MF);
#endif

  // Get the number of bytes to allocate from the FrameInfo.
  unsigned StackSize = MFI->getStackSize();

  // No need to allocate space on the stack.
  if (StackSize == 0 && !MFI->adjustsStack() && !requiresRA) return;

#if 0
  int FPOffset = OR1KFI->getFPStackOffset();
  int RAOffset = OR1KFI->getRAStackOffset();
#endif

  // Adjust stack : l.addi r1, r1, -imm
  if (StackSize) {
    BuildMI(MBB, MBBI, DL, TII.get(OR1K::ADDI), OR1K::R1)
      .addReg(OR1K::R1).addImm(-StackSize);
  }

#if 0
  // swi  R15, R1, stack_loc
  if (MFI->adjustsStack() || requiresRA) {
    BuildMI(MBB, MBBI, DL, TII.get(OR1K::SW))
        .addReg(OR1K::R9).addReg(OR1K::R1).addImm(RAOffset);
  }
  if (hasFP(MF)) {
    // swi  R19, R1, stack_loc
    BuildMI(MBB, MBBI, DL, TII.get(MBlaze::SWI))
      .addReg(MBlaze::R19).addReg(MBlaze::R1).addImm(FPOffset);

    // add R19, R1, R0
    BuildMI(MBB, MBBI, DL, TII.get(MBlaze::ADD), MBlaze::R19)
      .addReg(MBlaze::R1).addReg(MBlaze::R0);
  }
#endif
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
  bool requiresRA = false; //CallConv == llvm::CallingConv::MBLAZE_INTR;

#if 0
  // Get the FI's where RA and FP are saved.
  int FPOffset = OR1KFI->getFPStackOffset();
  int RAOffset = OR1KFI->getRAStackOffset();

  if (hasFP(MF)) {
    // add R1, R19, R0
    BuildMI(MBB, MBBI, dl, TII.get(MBlaze::ADD), MBlaze::R1)
      .addReg(MBlaze::R19).addReg(MBlaze::R0);

    // lwi  R19, R1, stack_loc
    BuildMI(MBB, MBBI, dl, TII.get(MBlaze::LWI), MBlaze::R19)
      .addReg(MBlaze::R1).addImm(FPOffset);
  }

  // lwi R15, R1, stack_loc
  if (MFI->adjustsStack() || requiresRA) {
    BuildMI(MBB, MBBI, dl, TII.get(OR1K::LWZ), OR1K::R9)
      .addReg(OR1K::R1).addImm(RAOffset);
  }
#endif

  // Get the number of bytes from FrameInfo
  int StackSize = (int) MFI->getStackSize();

  // l.addi r1, r1, imm
  if (StackSize) {
    BuildMI(MBB, MBBI, dl, TII.get(OR1K::ADDI), OR1K::R1)
      .addReg(OR1K::R1).addImm(StackSize);
  }

}
