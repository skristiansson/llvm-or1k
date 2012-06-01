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

  /// VarArgsFrameIndex - FrameIndex for start of varargs area.
  int VarArgsFrameIndex;

public:
  OR1KMachineFunctionInfo() {}

  explicit OR1KMachineFunctionInfo(MachineFunction &MF) {}
  int getVarArgsFrameIndex() const { return VarArgsFrameIndex; }
  void setVarArgsFrameIndex(int Index) { VarArgsFrameIndex = Index; }
};

} // End llvm namespace

#endif
