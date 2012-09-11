//===-- OR1KAsmBackend.cpp - OR1K Assembler Backend -----------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/OR1KMCTargetDesc.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCAsmLayout.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCELFSymbolFlags.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCSectionELF.h"
#include "llvm/MC/MCSectionMachO.h"
#include "llvm/MC/MCValue.h"
#include "llvm/ADT/Twine.h"
#include "llvm/Support/ELF.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

static unsigned getFixupKindSize(unsigned Kind) {
  switch (Kind) {
    default: llvm_unreachable("invalid fixup kind!");
    case FK_Data_1: return 1;
    case FK_Data_2: return 2;
    case FK_Data_4: return 4;
  }
}

namespace {

class OR1KAsmBackend : public MCAsmBackend {
public:
  OR1KAsmBackend(const Target &T)
    : MCAsmBackend() {
  }

  unsigned getNumFixupKinds() const {
    // FIXME: Is this correct?
    return 0;
  }

  bool mayNeedRelaxation(const MCInst &Inst) const;

  bool fixupNeedsRelaxation(const MCFixup &Fixup,
                            uint64_t Value,
                            const MCInstFragment *DF,
                            const MCAsmLayout &Layout) const;

  void relaxInstruction(const MCInst &Inst, MCInst &Res) const;

  bool writeNopData(uint64_t Count, MCObjectWriter *OW) const;

  unsigned getPointerSize() const {
    return 4;
  }
};

static unsigned getRelaxedOpcode(unsigned Op) {
  // FIXME: All opcodes are already their relaxed opcode?
  switch (Op) {
    default: return Op;
  }
}


bool OR1KAsmBackend::mayNeedRelaxation(const MCInst &Inst) const {
  // FIXME: Looking at the Code Emitter list of opcodes, it looks like none of
  // these need relaxation. Add a comparison for opcode if this is incorrect.
  return false;
}

bool OR1KAsmBackend::fixupNeedsRelaxation(const MCFixup &Fixup,
                                          uint64_t Value,
                                          const MCInstFragment *DF,
                                          const MCAsmLayout &Layout) const {
  // FIXME: Looking at the Code Emitter list of opcodes, it looks like none of
  // these need relaxation. Add a comparison for opcode if this is incorrect.
  return false;
}

void OR1KAsmBackend::relaxInstruction(const MCInst &Inst, MCInst &Res) const {
  // FIXME: From above comment, no instruction needs relaxation so this function
  // does nothing.
}

bool OR1KAsmBackend::writeNopData(uint64_t Count, MCObjectWriter *OW) const {
  if ((Count % 4) != 0)
    return false;

  for (uint64_t i = 0; i < Count; i += 4)
    OW->Write32(0x00000015);

  return true;
}
} // end anonymous namespace

namespace {
class ELFOR1KAsmBackend : public OR1KAsmBackend {
public:
  uint8_t OSABI;
  ELFOR1KAsmBackend(const Target &T, uint8_t _OSABI)
    : OR1KAsmBackend(T), OSABI(_OSABI) {}

  void applyFixup(const MCFixup &Fixup, char *Data, unsigned DataSize,
                  uint64_t Value) const;

  MCObjectWriter *createObjectWriter(raw_ostream &OS) const {
    return createOR1KELFObjectWriter(OS, OSABI);
  }
};

void ELFOR1KAsmBackend::applyFixup(const MCFixup &Fixup, char *Data,
                                   unsigned DataSize, uint64_t Value) const {
  // FIXME: Implement with real fixups used.
  unsigned Size = getFixupKindSize(Fixup.getKind());

  assert(Fixup.getOffset() + Size <= DataSize && "Invalid fixup offset!");
  switch(Size) {
    default: llvm_unreachable("Cannot fixup unknown value");
  }
}
} // end anonymous namespace

MCAsmBackend *llvm::createOR1KAsmBackend(const Target &T, StringRef TT) {
  Triple TheTriple(TT);

  if (TheTriple.isOSDarwin())
    assert(0 && "Mac not supported on OR1K");

  if (TheTriple.isOSWindows())
    assert(0 && "Windows not supported on OR1K");

  uint8_t OSABI = MCELFObjectTargetWriter::getOSABI(TheTriple.getOS());
  return new ELFOR1KAsmBackend(T, OSABI);
}