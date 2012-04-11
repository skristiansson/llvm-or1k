//===- OR1KSubtarget.cpp - OR1K Subtarget Information -----------*- C++ -*-=//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the OR1K specific subclass of TargetSubtarget.
//
//===----------------------------------------------------------------------===//

#include "OR1KSubtarget.h"
#include "OR1K.h"
#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "OR1KGenSubtargetInfo.inc"
using namespace llvm;

void OR1KSubtarget::anchor() { }

OR1KSubtarget::OR1KSubtarget(const std::string &TT,
                             const std::string &CPU, const std::string &FS)
  : OR1KGenSubtargetInfo(TT, CPU, FS)
{
}
