//=====-- OR1KMCAsmInfo.h - OR1K asm properties -----------*- C++ -*--====//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the OR1KMCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef MBLAZETARGETASMINFO_H
#define MBLAZETARGETASMINFO_H

#include "llvm/ADT/StringRef.h"
#include "llvm/MC/MCAsmInfo.h"

namespace llvm {
  class Target;
  
  class OR1KMCAsmInfo : public MCAsmInfo {
  public:
    explicit OR1KMCAsmInfo(const Target &T, StringRef TT);
  };

} // namespace llvm

#endif
