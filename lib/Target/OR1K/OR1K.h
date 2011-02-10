//===-- OR1K.h - Top-level interface for OR1K representation --*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in the LLVM
// OR1K back-end.
//
//===----------------------------------------------------------------------===//

#ifndef TARGET_XCORE_H
#define TARGET_XCORE_H

#include "llvm/Target/TargetMachine.h"

namespace llvm {
  class FunctionPass;
  class TargetMachine;
  class OR1KTargetMachine;
  class formatted_raw_ostream;

  FunctionPass *createOR1KISelDag(OR1KTargetMachine &TM);

  extern Target TheOR1KTarget;

} // end namespace llvm;

// Defines symbolic names for OR1K registers.  This defines a mapping from
// register name to register number.
//
#include "OR1KGenRegisterNames.inc"

// Defines symbolic names for the OR1K instructions.
//
#include "OR1KGenInstrNames.inc"

#endif
