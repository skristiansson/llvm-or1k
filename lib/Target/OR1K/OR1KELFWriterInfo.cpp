//===-- OR1KELFWriterInfo.cpp - ELF Writer Info for the OR1K backend ------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements ELF writer information for the OR1K backend.
//
//===----------------------------------------------------------------------===//

#include "OR1KELFWriterInfo.h"
#include "OR1KRelocations.h"
#include "llvm/Function.h"
#include "llvm/Support/ELF.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetData.h"
#include "llvm/Target/TargetMachine.h"

using namespace llvm;

OR1KELFWriterInfo::OR1KELFWriterInfo(TargetMachine &TM)
  : TargetELFWriterInfo(TM.getTargetData()->getPointerSizeInBits() == 64,
                        TM.getTargetData()->isLittleEndian()) {}

OR1KELFWriterInfo::~OR1KELFWriterInfo() {}

unsigned OR1KELFWriterInfo::getRelocationType(unsigned MachineRelTy) const {
  switch(MachineRelTy) {
    case OR1K::reloc_absolute_word:
      return ELF::R_OR32_NONE;
  }
}

long int OR1KELFWriterInfo::getDefaultAddendForRelTy(unsigned RelTy,
                                                  long int Modifier) const {
  // FIXME: What is the default addend?
  return Modifier;
}

unsigned OR1KELFWriterInfo::getRelocationTySize(unsigned RelTy) const {
  switch (RelTy) {
    case ELF::R_OR32_NONE:
      return 0;
    case ELF::R_OR32_8:
      return 8;
    case ELF::R_OR32_16:
    case ELF::R_OR32_CONST:
    case ELF::R_OR32_CONSTH:
      return 16;
    case ELF::R_OR32_JUMPTARG:
      return 28;
    case ELF::R_OR32_32:
      return 32;
  }

  return 0;
}

bool OR1KELFWriterInfo::isPCRelativeRel(unsigned RelTy) const {
  // FIXME: From the architecture manual, it looks like none are
  return false;
}

unsigned OR1KELFWriterInfo::getAbsoluteLabelMachineRelTy() const {
  return OR1K::reloc_absolute_word;
}

long int OR1KELFWriterInfo::computeRelocation(unsigned SymOffset,
                                              unsigned RelOffset,
                                              unsigned RelTy) const {
  // FIXME: Do any relocations used require this?
  return SymOffset;
}