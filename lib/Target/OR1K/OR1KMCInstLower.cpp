//=-- OR1KMCInstLower.cpp - Convert OR1K MachineInstr to an MCInst ----------=//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains code to lower OR1K MachineInstrs to their corresponding
// MCInst records.
//
//===----------------------------------------------------------------------===//

#include "OR1KMCInstLower.h"
#include "MCTargetDesc/OR1KBaseInfo.h"
#include "llvm/Constants.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/Target/Mangler.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/ADT/SmallString.h"
using namespace llvm;

MCSymbol *OR1KMCInstLower::
GetGlobalAddressSymbol(const MachineOperand &MO) const {
  switch (MO.getTargetFlags()) {
  default: llvm_unreachable("Unknown target flag on GV operand");
  case OR1KII::MO_NO_FLAG:
  case OR1KII::MO_ABS_HI:
  case OR1KII::MO_ABS_LO:
  case OR1KII::MO_PLT:
  case OR1KII::MO_GOTPCHI:
  case OR1KII::MO_GOTPCLO:
  case OR1KII::MO_GOTOFFHI:
  case OR1KII::MO_GOTOFFLO:
  case OR1KII::MO_GOT:
    break;
  }

  return Printer.Mang->getSymbol(MO.getGlobal());
}

MCSymbol *OR1KMCInstLower::
GetExternalSymbolSymbol(const MachineOperand &MO) const {
  switch (MO.getTargetFlags()) {
  default: llvm_unreachable("Unknown target flag on GV operand");
  case OR1KII::MO_NO_FLAG:
  case OR1KII::MO_ABS_HI:
  case OR1KII::MO_ABS_LO:
  case OR1KII::MO_PLT:
  case OR1KII::MO_GOTPCHI:
  case OR1KII::MO_GOTPCLO:
  case OR1KII::MO_GOTOFFHI:
  case OR1KII::MO_GOTOFFLO:
  case OR1KII::MO_GOT:
    break;
  }

  return Printer.GetExternalSymbolSymbol(MO.getSymbolName());
}

MCSymbol *OR1KMCInstLower::
GetJumpTableSymbol(const MachineOperand &MO) const {
  SmallString<256> Name;
  raw_svector_ostream(Name) << Printer.MAI->getPrivateGlobalPrefix() << "JTI"
                            << Printer.getFunctionNumber() << '_'
                            << MO.getIndex();

  switch (MO.getTargetFlags()) {
  default: llvm_unreachable("Unknown target flag on GV operand");
  case OR1KII::MO_NO_FLAG:
  case OR1KII::MO_ABS_HI:
  case OR1KII::MO_ABS_LO:
  case OR1KII::MO_PLT:
  case OR1KII::MO_GOTPCHI:
  case OR1KII::MO_GOTPCLO:
  case OR1KII::MO_GOTOFFHI:
  case OR1KII::MO_GOTOFFLO:
  case OR1KII::MO_GOT:
    break;
  }

  // Create a symbol for the name.
  return Ctx.GetOrCreateSymbol(Name.str());
}

MCSymbol *OR1KMCInstLower::
GetConstantPoolIndexSymbol(const MachineOperand &MO) const {
  SmallString<256> Name;
  raw_svector_ostream(Name) << Printer.MAI->getPrivateGlobalPrefix() << "CPI"
                            << Printer.getFunctionNumber() << '_'
                            << MO.getIndex();

  switch (MO.getTargetFlags()) {
  default: llvm_unreachable("Unknown target flag on GV operand");
  case OR1KII::MO_NO_FLAG:
  case OR1KII::MO_ABS_HI:
  case OR1KII::MO_ABS_LO:
  case OR1KII::MO_PLT:
  case OR1KII::MO_GOTPCHI:
  case OR1KII::MO_GOTPCLO:
  case OR1KII::MO_GOTOFFHI:
  case OR1KII::MO_GOTOFFLO:
  case OR1KII::MO_GOT:
    break;
  }

  // Create a symbol for the name.
  return Ctx.GetOrCreateSymbol(Name.str());
}

MCOperand OR1KMCInstLower::
LowerSymbolOperand(const MachineOperand &MO, MCSymbol *Sym) const {
  MCSymbolRefExpr::VariantKind Kind;

  switch (MO.getTargetFlags()) {
  default: llvm_unreachable("Unknown target flag on GV operand");
  case OR1KII::MO_NO_FLAG:  Kind = MCSymbolRefExpr::VK_None; break;
  case OR1KII::MO_ABS_HI:   Kind = MCSymbolRefExpr::VK_OR1K_ABS_HI; break;
  case OR1KII::MO_ABS_LO:   Kind = MCSymbolRefExpr::VK_OR1K_ABS_LO; break;
  case OR1KII::MO_PLT:      Kind = MCSymbolRefExpr::VK_OR1K_PLT; break;
  case OR1KII::MO_GOTPCHI:  Kind = MCSymbolRefExpr::VK_OR1K_GOTPCHI; break;
  case OR1KII::MO_GOTPCLO:  Kind = MCSymbolRefExpr::VK_OR1K_GOTPCLO; break;
  case OR1KII::MO_GOTOFFHI: Kind = MCSymbolRefExpr::VK_OR1K_GOTOFFHI; break;
  case OR1KII::MO_GOTOFFLO: Kind = MCSymbolRefExpr::VK_OR1K_GOTOFFLO; break;
  case OR1KII::MO_GOT:      Kind = MCSymbolRefExpr::VK_OR1K_GOT; break;
  }

  const MCExpr *Expr = MCSymbolRefExpr::Create(Sym, Kind, Ctx);

  if (!MO.isJTI() && MO.getOffset())
    Expr = MCBinaryExpr::CreateAdd(Expr,
                                   MCConstantExpr::Create(MO.getOffset(), Ctx),
                                   Ctx);
  return MCOperand::CreateExpr(Expr);
}

void OR1KMCInstLower::Lower(const MachineInstr *MI, MCInst &OutMI) const {
  OutMI.setOpcode(MI->getOpcode());

  for (unsigned i = 0, e = MI->getNumOperands(); i != e; ++i) {
    const MachineOperand &MO = MI->getOperand(i);

    MCOperand MCOp;
    switch (MO.getType()) {
    default:
      MI->dump();
      llvm_unreachable("unknown operand type");
    case MachineOperand::MO_Register:
      // Ignore all implicit register operands.
      if (MO.isImplicit()) continue;
      MCOp = MCOperand::CreateReg(MO.getReg());
      break;
    case MachineOperand::MO_FPImmediate: {
     APFloat Val = MO.getFPImm()->getValueAPF();
      // FP immediates are used only when setting GPRs, so they may be dealt
      // with like regular immediates from this point on.
      MCOp = MCOperand::CreateImm(*Val.bitcastToAPInt().getRawData());
      break;
    }
    case MachineOperand::MO_Immediate:
      MCOp = MCOperand::CreateImm(MO.getImm());
      break;
    case MachineOperand::MO_MachineBasicBlock:
      MCOp = MCOperand::CreateExpr(MCSymbolRefExpr::Create(
                                   MO.getMBB()->getSymbol(), Ctx));
      break;
    case MachineOperand::MO_RegisterMask:
      continue;
    case MachineOperand::MO_GlobalAddress:
      MCOp = LowerSymbolOperand(MO, GetGlobalAddressSymbol(MO));
      break;
    case MachineOperand::MO_ExternalSymbol:
      MCOp = LowerSymbolOperand(MO, GetExternalSymbolSymbol(MO));
      break;
    case MachineOperand::MO_JumpTableIndex:
      MCOp = LowerSymbolOperand(MO, GetJumpTableSymbol(MO));
      break;
    case MachineOperand::MO_ConstantPoolIndex:
      MCOp = LowerSymbolOperand(MO, GetConstantPoolIndexSymbol(MO));
      break;
    }

    OutMI.addOperand(MCOp);
  }
}
