//===-- OR1KFrameLowering.h - Define frame lowering for OR1K ---*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//
//
//===----------------------------------------------------------------------===//

#ifndef OR1K_FRAMEINFO_H
#define OR1K_FRAMEINFO_H

#include "OR1K.h"
#include "OR1KSubtarget.h"
#include "llvm/Target/TargetFrameLowering.h"

namespace llvm {
  class OR1KSubtarget;

class OR1KFrameLowering : public TargetFrameLowering {
private:
  void determineFrameLayout(MachineFunction &MF) const;
  void replaceAdjDynAllocPseudo(MachineFunction &MF) const;

protected:
  const OR1KSubtarget &STI;

public:
  explicit OR1KFrameLowering(const OR1KSubtarget &sti)
    : TargetFrameLowering(TargetFrameLowering::StackGrowsDown, 4, 0),
      STI(sti) {
  }

  /// emitProlog/emitEpilog - These methods insert prolog and epilog code into
  /// the function.
  void emitPrologue(MachineFunction &MF) const;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const;

  bool hasFP(const MachineFunction &MF) const;
  virtual void processFunctionBeforeCalleeSavedScan(MachineFunction &MF,
                                                    RegScavenger *RS) const;
};

} // End llvm namespace

#endif
