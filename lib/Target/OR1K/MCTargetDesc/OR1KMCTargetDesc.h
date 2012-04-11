//===-- OR1KMCTargetDesc.h - OR1K Target Descriptions ---------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides OR1K specific target descriptions.
//
//===----------------------------------------------------------------------===//

#ifndef SPARCMCTARGETDESC_H
#define SPARCMCTARGETDESC_H

namespace llvm {
class Target;

extern Target TheOR1KTarget;
extern Target TheOR1KV9Target;

} // End llvm namespace

// Defines symbolic names for OR1K registers.  This defines a mapping from
// register name to register number.
//
#define GET_REGINFO_ENUM
#include "OR1KGenRegisterInfo.inc"

// Defines symbolic names for the OR1K instructions.
//
#define GET_INSTRINFO_ENUM
#include "OR1KGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "OR1KGenSubtargetInfo.inc"

#endif
