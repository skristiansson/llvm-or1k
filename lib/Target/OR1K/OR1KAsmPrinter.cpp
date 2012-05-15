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

    void EmitInstruction(const MachineInstr *MI);
    virtual bool isBlockOnlyReachableByFallthrough(const MachineBasicBlock*
                                                   MBB) const;
  };
} // end of anonymous namespace


//===----------------------------------------------------------------------===//
void OR1KAsmPrinter::EmitInstruction(const MachineInstr *MI) {
  OR1KMCInstLower MCInstLowering(OutContext, *Mang, *this);

  MCInst TmpInst;
  MCInstLowering.Lower(MI, TmpInst);
  OutStreamer.EmitInstruction(TmpInst);
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
