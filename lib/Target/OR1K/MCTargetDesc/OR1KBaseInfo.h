//===-- OR1KBaseInfo.h - Top level definitions for OR1K MC ------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains small standalone helper functions and enum definitions for
// the OR1K target useful for the compiler back-end and the MC libraries.
//
//===----------------------------------------------------------------------===//
#ifndef OR1KBASEINFO_H
#define OR1KBASEINFO_H

#include "OR1KMCTargetDesc.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/Support/DataTypes.h"
#include "llvm/Support/ErrorHandling.h"

namespace llvm {

/// OR1KII - This namespace holds all of the target specific flags that
/// instruction info tracks.
///
namespace OR1KII {
  /// Target Operand Flag enum.
  enum TOF {
    //===------------------------------------------------------------------===//
    // OR1K Specific MachineOperand flags.

    MO_NO_FLAG,

    /// MO_ABS_HI/LO - Represents the hi or low part of an absolute symbol
    /// address.
    MO_ABS_HI,
    MO_ABS_LO,

    /// MO_PIC_BASE_OFFSET - On a symbol operand this indicates that the
    /// immediate should get the value of the symbol minus the PIC base label:
    ///    SYMBOL_LABEL - PICBASELABEL
    MO_PIC_BASE_OFFSET,

    /// MO_GOT - On a symbol operand this indicates that the immediate is the
    /// offset to the GOT entry for the symbol name from the base of the GOT.
    MO_GOT,

    /// MO_GOTOFFHI/MO_GOTOFFLO - On a symbol operand this indicates that the
    /// immediate is the offset to the location of the symbol name from the
    /// base of the GOT.
    MO_GOTOFFHI,
    MO_GOTOFFLO,

    /// MO_GOTPCHI/MO_GOTPCLO - On a symbol operand this indicates that
    /// the immediate is an offset to the GOT entry for the symbol name
    /// from the current code location.
    MO_GOTPCHI,
    MO_GOTPCLO,

    /// MO_PLT - On a symbol operand this indicates that the immediate is
    /// offset to the PLT entry of symbol name from the current code location.
    MO_PLT
  };
}

static inline unsigned getOR1KRegisterNumbering(unsigned Reg) {
  switch(Reg) {
    case OR1K::R0  : return 0;
    case OR1K::R1  : return 1;
    case OR1K::R2  : return 2;
    case OR1K::R3  : return 3;
    case OR1K::R4  : return 4;
    case OR1K::R5  : return 5;
    case OR1K::R6  : return 6;
    case OR1K::R7  : return 7;
    case OR1K::R8  : return 8;
    case OR1K::R9  : return 9;
    case OR1K::R10 : return 10;
    case OR1K::R11 : return 11;
    case OR1K::R12 : return 12;
    case OR1K::R13 : return 13;
    case OR1K::R14 : return 14;
    case OR1K::R15 : return 15;
    case OR1K::R16 : return 16;
    case OR1K::R17 : return 17;
    case OR1K::R18 : return 18;
    case OR1K::R19 : return 19;
    case OR1K::R20 : return 20;
    case OR1K::R21 : return 21;
    case OR1K::R22 : return 22;
    case OR1K::R23 : return 23;
    case OR1K::R24 : return 24;
    case OR1K::R25 : return 25;
    case OR1K::R26 : return 26;
    case OR1K::R27 : return 27;
    case OR1K::R28 : return 28;
    case OR1K::R29 : return 29;
    case OR1K::R30 : return 30;
    case OR1K::R31 : return 31;
    default: llvm_unreachable("Unknown register number!");
  }
}

}
#endif
