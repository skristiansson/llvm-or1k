//===-- OR1KFixupKinds.h - OR1K Specific Fixup Entries ----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef LLVM_OR1K_OR1KFIXUPKINDS_H
#define LLVM_OR1K_OR1KFIXUPKINDS_H

#include "llvm/MC/MCFixup.h"

namespace llvm {
namespace OR1K {
  // Although most of the current fixup types reflect a unique relocation
  // one can have multiple fixup types for a given relocation and thus need
  // to be uniquely named.
  //
  // This table *must* be in the save order of
  // MCFixupKindInfo Infos[OR1K::NumTargetFixupKinds]
  // in OR1KAsmBackend.cpp.
  //
  enum Fixups {
    // Results in R_OR1K_NONE
    fixup_OR1K_NONE = FirstTargetFixupKind,

    // Results in R_OR1K_32
    fixup_OR1K_32,

    // Results in R_OR1K_16
    fixup_OR1K_16,

    // Results in R_OR1K_8
    fixup_OR1K_8,

    // Results in R_OR1K_LO_16_IN_INSN
    fixup_OR1K_LO16_INSN,

    // Results in R_OR1K_HI_16_IN_INSN
    fixup_OR1K_HI16_INSN,

    // Results in R_OR1K_INSN_REL_26
    fixup_OR1K_REL26,

    // Results in R_OR1K_32_PCREL
    fixup_OR1K_PCREL32,

    // Results in R_OR1K_16_PCREL
    fixup_OR1K_PCREL16,

    // Results in R_OR1K_8_PCREL
    fixup_OR1K_PCREL8,

    // Results in R_OR1K_GOTPC_HI16
    fixup_OR1K_GOTPC_HI16,

    // Results in R_OR1K_GOTPC_LO16
    fixup_OR1K_GOTPC_LO16,

    // Results in R_OR1K_GOT16
    fixup_OR1K_GOT16,

    // Results in R_OR1K_PLT26
    fixup_OR1K_PLT26,

    // Results in R_OR1K_GOTOFF_HI16
    fixup_OR1K_GOTOFF_HI16,

    // Results in R_OR1K_GOTOFF_LO16
    fixup_OR1K_GOTOFF_LO16,

    // Results in R_OR1K_COPY
    fixup_OR1K_COPY,

    // Results in R_OR1K_GLOB_DAT
    fixup_OR1K_GLOB_DAT,

    // Results in R_OR1K_JMP_SLOT
    fixup_OR1K_JMP_SLOT,

    // Results in R_OR1K_RELATIVE
    fixup_OR1K_RELATIVE,

    // Marker
    LastTargetFixupKind,
    NumTargetFixupKinds = LastTargetFixupKind - FirstTargetFixupKind
  };
} // end namespace OR1K
} // end namespace llvm

#endif