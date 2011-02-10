//===- OR1KMachineFunctionInfo.h - OR1K Machine Function Info -*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares  OR1K specific per-machine-function information.
//
//===----------------------------------------------------------------------===//
#ifndef SPARCMACHINEFUNCTIONINFO_H
#define SPARCMACHINEFUNCTIONINFO_H

#include "llvm/CodeGen/MachineFunction.h"

namespace llvm {

  class OR1KMachineFunctionInfo : public MachineFunctionInfo {
  private:
/*
    unsigned GlobalBaseReg;

    /// VarArgsFrameOffset - Frame offset to start of varargs area.
    int VarArgsFrameOffset;
*/
  public:
    OR1KMachineFunctionInfo() {}
    explicit OR1KMachineFunctionInfo(MachineFunction &MF) {}

/*
    unsigned getGlobalBaseReg() const { return GlobalBaseReg; }
    void setGlobalBaseReg(unsigned Reg) { GlobalBaseReg = Reg; }

    int getVarArgsFrameOffset() const { return VarArgsFrameOffset; }
    void setVarArgsFrameOffset(int Offset) { VarArgsFrameOffset = Offset; }
*/
  };
}

#endif
