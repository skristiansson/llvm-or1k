//===-- OR1KSelectionDAGInfo.h - OR1K SelectionDAG Info -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the OR1K subclass for TargetSelectionDAGInfo.
//
//===----------------------------------------------------------------------===//

#ifndef OR1KSELECTIONDAGINFO_H
#define OR1KSELECTIONDAGINFO_H

#include "llvm/Target/TargetSelectionDAGInfo.h"

namespace llvm {

class OR1KTargetMachine;

class OR1KSelectionDAGInfo : public TargetSelectionDAGInfo {
public:
  explicit OR1KSelectionDAGInfo(const OR1KTargetMachine &TM);
  ~OR1KSelectionDAGInfo();
};

}

#endif
