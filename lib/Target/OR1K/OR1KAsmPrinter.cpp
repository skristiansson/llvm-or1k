//===-- OR1KAsmPrinter.cpp - OR1K LLVM assembly writer --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to the OR1K assembly language.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "asm-printer"
#include "OR1K.h"
#include "OR1KInstrInfo.h"
#include "OR1KMCInstLower.h"
#include "OR1KTargetMachine.h"
#include "InstPrinter/OR1KInstPrinter.h"
#include "llvm/Constants.h"
#include "llvm/DerivedTypes.h"
#include "llvm/Module.h"
#include "llvm/Assembly/Writer.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Target/Mangler.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

namespace {
  class OR1KAsmPrinter : public AsmPrinter {
  public:
    explicit OR1KAsmPrinter(TargetMachine &TM, MCStreamer &Streamer)
      : AsmPrinter(TM, Streamer) {}

    virtual const char *getPassName() const {
      return "OR1K Assembly Printer";
    }

    void printOperand(const MachineInstr *MI, int OpNum,
                      raw_ostream &O, const char* Modifier = 0);
    bool PrintAsmOperand(const MachineInstr *MI, unsigned OpNo,
                         unsigned AsmVariant, const char *ExtraCode,
                         raw_ostream &O);
    void EmitInstruction(const MachineInstr *MI);
    virtual bool isBlockOnlyReachableByFallthrough(const MachineBasicBlock*
                                                   MBB) const;
  private:
    void customEmitInstruction(const MachineInstr *MI);
  };
} // end of anonymous namespace

void OR1KAsmPrinter::printOperand(const MachineInstr *MI, int OpNum,
                                  raw_ostream &O, const char *Modifier) {
  const MachineOperand &MO = MI->getOperand(OpNum);
  unsigned TF = MO.getTargetFlags();

  switch (MO.getType()) {
  case MachineOperand::MO_Register:
    O << OR1KInstPrinter::getRegisterName(MO.getReg());
    break;

  case MachineOperand::MO_Immediate:
    O << MO.getImm();
    break;

  case MachineOperand::MO_MachineBasicBlock:
    O << *MO.getMBB()->getSymbol();
    break;

  case MachineOperand::MO_GlobalAddress:
    if (TF == OR1KII::MO_PLT)
      O << "plt(" << *Mang->getSymbol(MO.getGlobal()) << ")";
    else
      O << *Mang->getSymbol(MO.getGlobal());
    break;

  case MachineOperand::MO_BlockAddress: {
     MCSymbol* BA = GetBlockAddressSymbol(MO.getBlockAddress());
     O << BA->getName();
     break;
   }

   case MachineOperand::MO_ExternalSymbol:
     if (TF == OR1KII::MO_PLT)
       O << "plt(" << *GetExternalSymbolSymbol(MO.getSymbolName()) << ")";
     else
       O << *GetExternalSymbolSymbol(MO.getSymbolName());
     break;

   case MachineOperand::MO_JumpTableIndex:
     O << MAI->getPrivateGlobalPrefix() << "JTI" << getFunctionNumber()
       << '_' << MO.getIndex();
     break;

  case MachineOperand::MO_ConstantPoolIndex:
    O << MAI->getPrivateGlobalPrefix() << "CPI" << getFunctionNumber()
      << '_' << MO.getIndex();
    return;

  default:
    llvm_unreachable("<unknown operand type>");
  }
}

/// PrintAsmOperand - Print out an operand for an inline asm expression.
///
bool OR1KAsmPrinter::PrintAsmOperand(const MachineInstr *MI, unsigned OpNo,
                                     unsigned AsmVariant,
                                     const char *ExtraCode, raw_ostream &O) {
  // Does this asm operand have a single letter operand modifier?
  if (ExtraCode && ExtraCode[0]) {
    if (ExtraCode[1])
      return true; // Unknown modifier.

    switch (ExtraCode[0]) {
    default:
      return true; // Unknown modifier.
    case 'H': // The highest-numbered register of a pair.
      if (OpNo == 0)
        return true;
      const MachineOperand &FlagsOP = MI->getOperand(OpNo - 1);
      if (!FlagsOP.isImm())
        return true;
      unsigned Flags = FlagsOP.getImm();
      unsigned NumVals = InlineAsm::getNumOperandRegisters(Flags);
      if (NumVals != 2)
        return true;
      unsigned RegOp = OpNo + 1;
      if (RegOp >= MI->getNumOperands())
        return true;
      const MachineOperand &MO = MI->getOperand(RegOp);
      if (!MO.isReg())
        return true;
      unsigned Reg = MO.getReg();
      O << OR1KInstPrinter::getRegisterName(Reg);
      return false;
    }
  }
  printOperand(MI, OpNo, O);
  return false;
}

//===----------------------------------------------------------------------===//
void OR1KAsmPrinter::customEmitInstruction(const MachineInstr *MI) {
  OR1KMCInstLower MCInstLowering(OutContext, *Mang, *this);
  unsigned Opcode = MI->getOpcode();

  switch (Opcode) {
  default: break;
  case OR1K::MOVHI:
  case OR1K::ORI: {
    MCSymbolRefExpr::VariantKind Kind = MCSymbolRefExpr::VK_None;
    if (Opcode == OR1K::MOVHI &&
        MI->getOperand(1).getTargetFlags() == OR1KII::MO_GOTPCHI)
      Kind = MCSymbolRefExpr::VK_OR1K_GOTPCHI;
    else if (Opcode == OR1K::ORI &&
             MI->getOperand(2).getTargetFlags() == OR1KII::MO_GOTPCLO)
      Kind = MCSymbolRefExpr::VK_OR1K_GOTPCLO;
    else
      break;

    // We want to print something like:
    //   MYGLOBAL + (. - PICBASE)
    // However, we can't generate a ".", so just emit a new label here and refer
    // to it.
    MCSymbol *DotSym = OutContext.CreateTempSymbol();
    const MCExpr *DotExpr = MCSymbolRefExpr::Create(DotSym, OutContext);
    const MCExpr *PICBase =
      MCSymbolRefExpr::Create(MF->getPICBaseSymbol(), OutContext);

    OutStreamer.EmitLabel(DotSym);

    // Now that we have emitted the label, lower the complex operand expression.
    MachineOperand MO = (MI->getOpcode() == OR1K::MOVHI) ?
      MI->getOperand(1) : MI->getOperand(2);
    MCSymbol *OpSym = MCInstLowering.GetExternalSymbolSymbol(MO);

    DotExpr = MCBinaryExpr::CreateSub(DotExpr, PICBase, OutContext);

    DotExpr = MCBinaryExpr::CreateAdd(MCSymbolRefExpr::Create(OpSym, Kind,
                                                              OutContext),
                                      DotExpr, OutContext);

    MCInst TmpInst;
    TmpInst.setOpcode(MI->getOpcode());
    TmpInst.addOperand(MCOperand::CreateReg(MI->getOperand(0).getReg()));
    if (MI->getOpcode() == OR1K::ORI)
      TmpInst.addOperand(MCOperand::CreateReg(MI->getOperand(1).getReg()));
    TmpInst.addOperand(MCOperand::CreateExpr(DotExpr));
    OutStreamer.EmitInstruction(TmpInst);
    return;
  }

  case OR1K::GETPC: {
    MCInst TmpInst;
    // This is a pseudo op for a two instruction sequence with a label, which
    // looks like:
    //     l.jal .L1$pb
    //     l.nop
    // .L1$pb:

    // Emit the call.
    MCSymbol *PICBase = MF->getPICBaseSymbol();
    TmpInst.setOpcode(OR1K::JAL);
    // FIXME: We would like an efficient form for this, so we don't have to do a
    // lot of extra uniquing.
    TmpInst.addOperand(MCOperand::CreateExpr(
                         MCSymbolRefExpr::Create(PICBase,OutContext)));
    OutStreamer.EmitInstruction(TmpInst);

    // Emit delay-slot nop
    // FIXME: omit on no-delay-slot targets
    TmpInst.setOpcode(OR1K::NOP);
    TmpInst.getOperand(0) = MCOperand::CreateImm(0);
    OutStreamer.EmitInstruction(TmpInst);

    // Emit the label.
    OutStreamer.EmitLabel(PICBase);

    return;
  }
  }

  MCInst TmpInst;
  MCInstLowering.Lower(MI, TmpInst);
  OutStreamer.EmitInstruction(TmpInst);
}

void OR1KAsmPrinter::EmitInstruction(const MachineInstr *MI) {

  MachineBasicBlock::const_instr_iterator I = MI;
  MachineBasicBlock::const_instr_iterator E = MI->getParent()->instr_end();

  do {
    customEmitInstruction(I++);
  } while ((I != E) && I->isInsideBundle());
}

/// isBlockOnlyReachableByFallthough - Return true if the basic block has
/// exactly one predecessor and the control transfer mechanism between
/// the predecessor and this block is a fall-through.
// FIXME: could the overridden cases be handled in AnalyzeBranch?
bool OR1KAsmPrinter::isBlockOnlyReachableByFallthrough(const MachineBasicBlock*
                                                       MBB) const {
  // The predecessor has to be immediately before this block.
  const MachineBasicBlock *Pred = *MBB->pred_begin();

  // If the predecessor is a switch statement, assume a jump table
  // implementation, so it is not a fall through.
  if (const BasicBlock *bb = Pred->getBasicBlock())
    if (isa<SwitchInst>(bb->getTerminator()))
      return false;

  // Check default implementation
  if (!AsmPrinter::isBlockOnlyReachableByFallthrough(MBB))
    return false;

  // Otherwise, check the last instruction.
  // Check if the last terminator is an unconditional branch.
  MachineBasicBlock::const_iterator I = Pred->end();
  while (I != Pred->begin() && !(--I)->isTerminator()) ;

  return !I->isBarrier();
}

// Force static initialization.
extern "C" void LLVMInitializeOR1KAsmPrinter() {
  RegisterAsmPrinter<OR1KAsmPrinter> X(TheOR1KTarget);
}
