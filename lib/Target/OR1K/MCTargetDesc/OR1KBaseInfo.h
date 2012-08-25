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
// the Mips target useful for the compiler back-end and the MC libraries.
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
}
#endif
