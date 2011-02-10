//===-- OR1KTargetInfo.cpp - OR1K Target Implementation -----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "OR1K.h"
#include "llvm/Module.h"
#include "llvm/Target/TargetRegistry.h"
using namespace llvm;

Target llvm::TheOR1KTarget;

extern "C" void LLVMInitializeOR1KTargetInfo() { 
  RegisterTarget<Triple::or1k> X(TheOR1KTarget, "or1k", "OR1K");
}
