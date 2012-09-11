//===-- OR1KMCTargetDesc.h - OR1K Target Descriptions -----------*- C++ -*-===//
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

#ifndef OR1KMCTARGETDESC_H
#define OR1KMCTARGETDESC_H

#include "llvm/Support/DataTypes.h"

namespace llvm {
class MCAsmBackend;
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCObjectWriter;
class MCRegisterInfo;
class MCSubtargetInfo;
class Target;
class StringRef;
class raw_ostream;

extern Target TheOR1KTarget;

MCCodeEmitter *createOR1KMCCodeEmitter(const MCInstrInfo &MCII,
                                       const MCRegisterInfo &MRI,
                                       const MCSubtargetInfo &STI,
                                       MCContext &Ctx);

MCAsmBackend *createOR1KAsmBackend(const Target &T, StringRef TT);

MCObjectWriter *createOR1KELFObjectWriter(raw_ostream &OS, uint8_t OSABI);
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
