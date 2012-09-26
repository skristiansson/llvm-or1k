//===-- OR1KMCCodeEmitter.cpp - Convert OR1K code to machine code ---------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the OR1KMCCodeEmitter class.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "mccodeemitter"
#include "MCTargetDesc/OR1KBaseInfo.h"
#include "MCTargetDesc/OR1KFixupKinds.h"
#include "MCTargetDesc/OR1KMCTargetDesc.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

STATISTIC(MCNumEmitted, "Number of MC instructions emitted");

namespace {
class OR1KMCCodeEmitter : public MCCodeEmitter {
  OR1KMCCodeEmitter(const OR1KMCCodeEmitter &); // DO NOT IMPLEMENT
  void operator=(const OR1KMCCodeEmitter &); // DO NOT IMPLEMENT
  const MCInstrInfo &MCII;
  const MCSubtargetInfo &STI;
  MCContext &Ctx;

public:
  OR1KMCCodeEmitter(const MCInstrInfo &mcii, const MCSubtargetInfo &sti,
                    MCContext &ctx)
    : MCII(mcii), STI(sti), Ctx(ctx) {
    }

  ~OR1KMCCodeEmitter() {}

  // getBinaryCodeForInstr - TableGen'erated function for getting the
  // binary encoding for an instruction.
  uint64_t getBinaryCodeForInstr(const MCInst &MI,
                                 SmallVectorImpl<MCFixup> &Fixups) const;

   // getMachineOpValue - Return binary encoding of operand. If the machin
   // operand requires relocation, record the relocation and return zero.
  unsigned getMachineOpValue(const MCInst &MI,const MCOperand &MO,
                             SmallVectorImpl<MCFixup> &Fixups) const;

  unsigned getMemoryOpValue(const MCInst &MI, unsigned Op,
                            SmallVectorImpl<MCFixup> &Fixups) const;

  // Emit one byte through output stream (from MCBlazeMCCodeEmitter)
  void EmitByte(unsigned char C, unsigned &CurByte, raw_ostream &OS) const {
    OS << (char)C;
    ++CurByte;
  }

  // Emit a series of bytes (little endian) (from MCBlazeMCCodeEmitter)
  void EmitLEConstant(uint64_t Val, unsigned Size, unsigned &CurByte,
                    raw_ostream &OS) const {
    assert(Size <= 8 && "size too big in emit constant");

    for (unsigned i = 0; i != Size; ++i) {
      EmitByte(Val & 255, CurByte, OS);
      Val >>= 8;
    }
  }

  // Emit a series of bytes (big endian)
  void EmitBEConstant(uint64_t Val, unsigned Size, unsigned &CurByte,
                      raw_ostream &OS) const {
    assert(Size <= 8 && "size too big in emit constant");

    for (int i = (Size-1)*8; i >= 0; i-=8)
      EmitByte((Val >> i) & 255, CurByte, OS);
  }

  void EncodeInstruction(const MCInst &MI, raw_ostream &OS,
                         SmallVectorImpl<MCFixup> &Fixups) const;
};
} // end anonymous namepsace

MCCodeEmitter *llvm::createOR1KMCCodeEmitter(const MCInstrInfo &MCII,
                                             const MCRegisterInfo &MRI,
                                             const MCSubtargetInfo &STI,
                                             MCContext &Ctx) {
  return new OR1KMCCodeEmitter(MCII, STI, Ctx);
}

/// getMachineOpValue - Return binary encoding of operand. If the machine
/// operand requires relocation, record the relocation and return zero.
unsigned OR1KMCCodeEmitter::
getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                  SmallVectorImpl<MCFixup> &Fixups) const {
  if (MO.isReg())
    return getOR1KRegisterNumbering(MO.getReg());
  if (MO.isImm())
    return static_cast<unsigned>(MO.getImm());
  
  // MO must be an expression
  assert(MO.isExpr());

  const MCExpr *Expr = MO.getExpr();
  MCExpr::ExprKind Kind = Expr->getKind();

  if (Kind == MCExpr::Binary) {
    Expr = static_cast<const MCBinaryExpr*>(Expr)->getLHS();
    Kind = Expr->getKind();
  }

  assert (Kind == MCExpr::SymbolRef);

  OR1K::Fixups FixupKind = OR1K::Fixups(0);

  switch(cast<MCSymbolRefExpr>(Expr)->getKind()) {
    default: llvm_unreachable("Unknown fixup kind!");
      break;
    // FIXME: We shouldn't have VK_None at this point?
    // Simple test seems to show that None here is REL26?
    case MCSymbolRefExpr::VK_None:
      FixupKind = OR1K::fixup_OR1K_REL26;
      break;
    case MCSymbolRefExpr::VK_OR1K_ABS_HI:
      FixupKind = OR1K::fixup_OR1K_HI16_INSN;
      break;
    case MCSymbolRefExpr::VK_OR1K_ABS_LO:
      FixupKind = OR1K::fixup_OR1K_LO16_INSN;
      break;
    case MCSymbolRefExpr::VK_OR1K_PLT:
      FixupKind = OR1K::fixup_OR1K_PLT26;
      break;
    case MCSymbolRefExpr::VK_OR1K_GOTPCHI:
      FixupKind = OR1K::fixup_OR1K_GOTPC_HI16;
      break;
    case MCSymbolRefExpr::VK_OR1K_GOTPCLO:
      FixupKind = OR1K::fixup_OR1K_GOTPC_LO16;
      break;
    case MCSymbolRefExpr::VK_OR1K_GOTOFFHI:
      FixupKind = OR1K::fixup_OR1K_GOTOFF_HI16;
      break;
    case MCSymbolRefExpr::VK_OR1K_GOTOFFLO:
      FixupKind = OR1K::fixup_OR1K_GOTOFF_LO16;
      break;
    case MCSymbolRefExpr::VK_OR1K_GOT:
      FixupKind = OR1K::fixup_OR1K_GOT16;
      break;
  }

  // Push fixup (all info is contained within)
  Fixups.push_back(MCFixup::Create(0, MO.getExpr(), MCFixupKind(FixupKind)));
  return 0;
}

void OR1KMCCodeEmitter::
EncodeInstruction(const MCInst &MI, raw_ostream &OS,
                         SmallVectorImpl<MCFixup> &Fixups) const {
  unsigned Opcode = MI.getOpcode();
  const MCInstrDesc &Desc = MCII.get(Opcode);
  // Keep track of the current byte being emitted
  unsigned CurByte = 0;

  // Get instruction encoding and emit it
  ++MCNumEmitted;       // Keep track of the number of emitted insns.
  unsigned Value = getBinaryCodeForInstr(MI, Fixups);
  EmitBEConstant(Value, 4, CurByte, OS);
}

// Encode OR1K Memory Operand
unsigned OR1KMCCodeEmitter::
getMemoryOpValue(const MCInst &MI, unsigned Op,
  SmallVectorImpl<MCFixup> &Fixups) const {
  unsigned encoding;
  const MCOperand op1 = MI.getOperand(1);
  assert(op1.isReg() && "First operand is not register.");
  encoding = (getOR1KRegisterNumbering(op1.getReg()) << 16);
  MCOperand op2 = MI.getOperand(2);
  assert(op2.isImm() && "Second operand is not immediate.");
  encoding |= (static_cast<short>(op2.getImm()) & 0xffff);
  return encoding;
}

#include "OR1KGenMCCodeEmitter.inc"
