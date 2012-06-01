//===-- OR1KMCAsmInfo.cpp - OR1K asm properties -----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declarations of the OR1KMCAsmInfo properties.
//
//===----------------------------------------------------------------------===//

#include "OR1KMCAsmInfo.h"
using namespace llvm;

OR1KMCAsmInfo::OR1KMCAsmInfo(const Target &T, StringRef TT) {
  HasSetDirective             = false;
  PrivateGlobalPrefix         = ".L";
  WeakRefDirective            = "\t.weak\t";

  // OR1K assembly requires ".section" before ".bss"
  UsesELFSectionDirectiveForBSS = true;
}
