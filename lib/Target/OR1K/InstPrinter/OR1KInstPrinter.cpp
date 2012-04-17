//===-- OR1KInstPrinter.cpp - Convert OR1K MCInst to asm syntax -----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This class prints an OR1K MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "asm-printer"
#include "OR1K.h"
#include "OR1KInstPrinter.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
using namespace llvm;


// Include the auto-generated portion of the assembly writer.
#include "OR1KGenAsmWriter.inc"

void OR1KInstPrinter::printInst(const MCInst *MI, raw_ostream &O,
                                StringRef Annot) {
  printInstruction(MI, O);
  printAnnotation(O, Annot);
}

void OR1KInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                   raw_ostream &O, const char *Modifier) {
  assert((Modifier == 0 || Modifier[0] == 0) && "No modifiers supported");
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isReg()) {
    O << getRegisterName(Op.getReg());
  } else if (Op.isImm()) {
    O << Op.getImm();
  } else
    assert(0 && "Unknown operand in printOperand");
}

void OR1KInstPrinter::printMemOperand(const MCInst *MI, int OpNo,
                                      raw_ostream &O, const char *Modifier) {
  const MCOperand &RegOp = MI->getOperand(OpNo);
  const MCOperand &OffsetOp = MI->getOperand(OpNo+1);
  // offset
  assert(OffsetOp.isImm() && "Offset operand not an immediate");
  O << OffsetOp.getImm();
  // register
  assert(RegOp.isReg() && "Register operand not a register");
  O  << "(" << getRegisterName(RegOp.getReg()) << ")";
}
