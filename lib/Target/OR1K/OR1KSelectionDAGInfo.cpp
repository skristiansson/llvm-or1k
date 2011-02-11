//===-- OR1KSelectionDAGInfo.cpp - OR1K SelectionDAG Info -------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the OR1KSelectionDAGInfo class.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "or1k-selectiondag-info"
#include "OR1KTargetMachine.h"
using namespace llvm;

OR1KSelectionDAGInfo::OR1KSelectionDAGInfo(const OR1KTargetMachine &TM)
  : TargetSelectionDAGInfo(TM) {
}

OR1KSelectionDAGInfo::~OR1KSelectionDAGInfo() {
}
