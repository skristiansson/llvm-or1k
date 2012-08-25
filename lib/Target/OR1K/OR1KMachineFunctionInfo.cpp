//===-- OR1KMachineFuctionInfo.cpp - OR1K machine function info ---===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "OR1KMachineFunctionInfo.h"

using namespace llvm;

void OR1KMachineFunctionInfo::anchor() { }

unsigned OR1KMachineFunctionInfo::getGlobalBaseReg() {
  // Return if it has already been initialized.
  if (GlobalBaseReg)
    return GlobalBaseReg;

  return GlobalBaseReg = MF.getRegInfo()
    .createVirtualRegister(&OR1K::GPRRegClass);
}
