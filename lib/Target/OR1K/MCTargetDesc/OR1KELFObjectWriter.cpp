//===-- OR1KELFObjectWriter.cpp - OR1K ELF Writer -------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/OR1KBaseInfo.h"
#include "MCTargetDesc/OR1KFixupKinds.h"
#include "MCTargetDesc/OR1KMCTargetDesc.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
  class OR1KELFObjectWriter : public MCELFObjectTargetWriter {
  public:
    OR1KELFObjectWriter(uint8_t OSABI);

    virtual ~OR1KELFObjectWriter();
  protected:
    virtual unsigned GetRelocType(const MCValue &Target, const MCFixup &Fixup,
                                  bool IsPCRel, bool IsRelocWithSymbol,
                                  int64_t Addend) const;
  };
}

OR1KELFObjectWriter::OR1KELFObjectWriter(uint8_t OSABI)
  : MCELFObjectTargetWriter(/*Is64Bit*/ false, OSABI, ELF::EM_OPENRISC,
                            /*HasRelocationAddend*/ true) {}

OR1KELFObjectWriter::~OR1KELFObjectWriter() {}

unsigned OR1KELFObjectWriter::GetRelocType(const MCValue &Target,
                                           const MCFixup &Fixup,
                                           bool IsPCRel,
                                           bool IsRelocWithSymbol,
                                           int64_t Addend) const {
  unsigned Type;
  unsigned Kind = (unsigned)Fixup.getKind();
  switch (Kind) {
    default: llvm_unreachable("Invalid fixup kind!");
    case OR1K::fixup_OR1K_PCREL32:
    case FK_PCRel_4:
      Type = ELF::R_OR1K_32_PCREL;
      break;
    case OR1K::fixup_OR1K_PCREL16:
    case FK_PCRel_2:
      Type = ELF::R_OR1K_16_PCREL;
      break;
    case OR1K::fixup_OR1K_PCREL8:
    case FK_PCRel_1:
      Type = ELF::R_OR1K_8_PCREL;
      break;
    case OR1K::fixup_OR1K_32:
    case FK_Data_4:
      Type = ELF::R_OR1K_32;
      break;
    case OR1K::fixup_OR1K_16:
    case FK_Data_2:
      Type = ELF::R_OR1K_16;
      break;
    case OR1K::fixup_OR1K_8:
    case FK_Data_1:
      Type = ELF::R_OR1K_8;
      break;
    case OR1K::fixup_OR1K_NONE:
      Type = ELF::R_OR1K_NONE;
      break;
    case OR1K::fixup_OR1K_LO16_INSN:
      Type = ELF::R_OR1K_LO_16_IN_INSN;
      break;
    case OR1K::fixup_OR1K_HI16_INSN:
      Type = ELF::R_OR1K_HI_16_IN_INSN;
      break;
    case OR1K::fixup_OR1K_REL26:
      Type = ELF::R_OR1K_INSN_REL_26;
      break;
    case OR1K::fixup_OR1K_GOTPC_HI16:
      Type = ELF::R_OR1K_GOTPC_HI16;
      break;
    case OR1K::fixup_OR1K_GOTPC_LO16:
      Type = ELF::R_OR1K_GOTPC_LO16;
      break;
    case OR1K::fixup_OR1K_GOT16:
      Type = ELF::R_OR1K_GOT16;
      break;
    case OR1K::fixup_OR1K_PLT26:
      Type = ELF::R_OR1K_PLT26;
      break;
    case OR1K::fixup_OR1K_GOTOFF_HI16:
      Type = ELF::R_OR1K_GOTOFF_HI16;
      break;
    case OR1K::fixup_OR1K_GOTOFF_LO16:
      Type = ELF::R_OR1K_GOTOFF_LO16;
      break;
    case OR1K::fixup_OR1K_COPY:
      Type = ELF::R_OR1K_COPY;
      break;
    case OR1K::fixup_OR1K_GLOB_DAT:
      Type = ELF::R_OR1K_GLOB_DAT;
      break;
    case OR1K::fixup_OR1K_JMP_SLOT:
      Type = ELF::R_OR1K_JMP_SLOT;
      break;
    case OR1K::fixup_OR1K_RELATIVE:
      Type = ELF::R_OR1K_RELATIVE;
      break;
  }
  return Type;
}

MCObjectWriter *llvm::createOR1KELFObjectWriter(raw_ostream &OS,
                                                uint8_t OSABI) {
  MCELFObjectTargetWriter *MOTW = new OR1KELFObjectWriter(OSABI);
  return createELFObjectWriter(MOTW, OS, /*IsLittleEndian=*/ false);
}