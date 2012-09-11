//===-- OR1KELFObjectWriter.cpp - OR1K ELF Writer -------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/OR1KMCTargetDesc.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixup.h"
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
  switch ((unsigned)Fixup.getKind()) {
    default: llvm_unreachable("Invalid fixup kind!");
    case FK_Data_4:
      Type = ELF::R_OR32_32;
      break;
    case FK_Data_2:
      Type = ELF::R_OR32_16;
      break;
    case FK_Data_1:
      Type = ELF::R_OR32_8;
      break;
  }
  return Type;
}

MCObjectWriter *llvm::createOR1KELFObjectWriter(raw_ostream &OS,
                                                uint8_t OSABI) {
  MCELFObjectTargetWriter *MOTW = new OR1KELFObjectWriter(OSABI);
  return createELFObjectWriter(MOTW, OS, /*IsLittleEndian=*/ false);
}