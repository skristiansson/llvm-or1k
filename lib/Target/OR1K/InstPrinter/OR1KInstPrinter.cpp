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
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCSymbol.h"
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

static void printExpr(const MCExpr *Expr, raw_ostream &O) {
  const MCSymbolRefExpr *SRE;

  if (const MCBinaryExpr *BE = dyn_cast<MCBinaryExpr>(Expr))
    SRE = dyn_cast<MCSymbolRefExpr>(BE->getLHS());
  else
    SRE = dyn_cast<MCSymbolRefExpr>(Expr);
  assert(SRE && "Unexpected MCExpr type.");

  MCSymbolRefExpr::VariantKind Kind = SRE->getKind();

  if (Kind == MCSymbolRefExpr::VK_None) {
    O << *Expr;
    return;
  }

  O <<  MCSymbolRefExpr::getVariantKindName(Kind) << "(";
  O << *Expr;
  O << ")";
}

void OR1KInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                   raw_ostream &O, const char *Modifier) {
  assert((Modifier == 0 || Modifier[0] == 0) && "No modifiers supported");
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isReg()) {
    O << getRegisterName(Op.getReg());
  } else if (Op.isImm()) {
    O << (int32_t)Op.getImm();
  } else {
    assert(Op.isExpr() && "Expected an expression");
    printExpr(Op.getExpr(), O);
  }
}

void OR1KInstPrinter::printMemOperand(const MCInst *MI, int OpNo,
                                      raw_ostream &O, const char *Modifier) {
  const MCOperand &RegOp = MI->getOperand(OpNo);
  const MCOperand &OffsetOp = MI->getOperand(OpNo+1);
  // offset
  if (OffsetOp.isImm()) {
    O << OffsetOp.getImm();
  } else {
    assert(OffsetOp.isExpr() && "Expected an expression");
    printExpr(OffsetOp.getExpr(), O);
  }
  // register
  assert(RegOp.isReg() && "Register operand not a register");
  O  << "(" << getRegisterName(RegOp.getReg()) << ")";
}

void OR1KInstPrinter::printS16ImmOperand(const MCInst *MI, unsigned OpNo,
                                         raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);
  assert(Op.isImm() && "Immediate operand not an immediate");
  O << (int16_t)Op.getImm();
}
