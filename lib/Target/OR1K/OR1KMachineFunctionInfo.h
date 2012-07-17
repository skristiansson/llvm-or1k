//===- OR1KMachineFuctionInfo.h - OR1K machine func info ---------*- C++ -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares OR1K-specific per-machine-function information.
//
//===----------------------------------------------------------------------===//

#ifndef OR1KMACHINEFUNCTIONINFO_H
#define OR1KMACHINEFUNCTIONINFO_H

#include "llvm/CodeGen/MachineFunction.h"

namespace llvm {

/// OR1KMachineFunctionInfo - This class is derived from MachineFunction and
/// contains private OR1K target-specific information for each MachineFunction.
class OR1KMachineFunctionInfo : public MachineFunctionInfo {
  virtual void anchor();

  /// SRetReturnReg - OR1K ABI require that sret lowering includes
  /// returning the value of the returned struct in a register. This field
  /// holds the virtual register into which the sret argument is passed.
  unsigned SRetReturnReg;

  /// VarArgsFrameIndex - FrameIndex for start of varargs area.
  int VarArgsFrameIndex;

public:
  OR1KMachineFunctionInfo() : SRetReturnReg(0),
                              VarArgsFrameIndex(0) {}

  explicit OR1KMachineFunctionInfo(MachineFunction &MF)
    : SRetReturnReg(0),
      VarArgsFrameIndex(0) {}

  unsigned getSRetReturnReg() const { return SRetReturnReg; }
  void setSRetReturnReg(unsigned Reg) { SRetReturnReg = Reg; }

  int getVarArgsFrameIndex() const { return VarArgsFrameIndex; }
  void setVarArgsFrameIndex(int Index) { VarArgsFrameIndex = Index; }
};

} // End llvm namespace

#endif
