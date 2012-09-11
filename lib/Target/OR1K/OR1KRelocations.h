//===-- OR1KRelocations.h - OR1K Code Relocations ---------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the OR1K target-specific relocation types.
//
//===----------------------------------------------------------------------===//

#ifndef OR1KRELOCATIONS_H
#define OR1KRELOCATIONS_H

#include "llvm/CodeGen/MachineRelocation.h"

namespace llvm {
  namespace OR1K {
    // FIXME: Add relocation types
    enum RelocationType {
      /// reloc_absolute_word - absolute relocation, just add the relocated
      /// value to the value already in memory.
      /// (From MBlaze this relates to none?)
      reloc_absolute_word = 0
    };
  }
}

#endif
