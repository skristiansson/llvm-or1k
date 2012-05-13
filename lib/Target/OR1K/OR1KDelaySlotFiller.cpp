//===-- OR1kDelaySlotFiller.cpp - OR1K delay slot filler ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Simple pass to fills delay slots with useful instructions.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "delay-slot-filler"

#include "OR1K.h"
#include "OR1KTargetMachine.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Target/TargetInstrInfo.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/ADT/Statistic.h"

using namespace llvm;

STATISTIC(FilledSlots, "Number of delay slots filled");

static cl::opt<bool> DisableDelaySlotFiller(
  "disable-or1k-delay-filler",
  cl::init(false),
  cl::desc("Fill the OR1K delay slots with useful instructions."),
  cl::Hidden);

namespace {
  struct Filler : public MachineFunctionPass {
    /// Target machine description which we query for reg. names, data
    /// layout, etc.
    ///
    TargetMachine &TM;
    const TargetInstrInfo *TII;
    MachineBasicBlock::iterator LastFiller;

    static char ID;
    Filler(TargetMachine &tm)
      : MachineFunctionPass(ID), TM(tm), TII(tm.getInstrInfo()) { }

    virtual const char *getPassName() const {
      return "OR1K Delay Slot Filler";
    }

    bool runOnMachineBasicBlock(MachineBasicBlock &MBB);
    bool runOnMachineFunction(MachineFunction &F) {
      bool Changed = false;
      for (MachineFunction::iterator FI = F.begin(), FE = F.end();
           FI != FE; ++FI)
        Changed |= runOnMachineBasicBlock(*FI);
      return Changed;
    }

    void insertDefsUses(MachineBasicBlock::iterator MI,
                        SmallSet<unsigned, 32>& RegDefs,
                        SmallSet<unsigned, 32>& RegUses);

    bool IsRegInSet(SmallSet<unsigned, 32>& RegSet,
                    unsigned Reg);

    bool delayHasHazard(MachineBasicBlock::iterator MI,
                        bool &sawLoad, bool &sawStore,
                        SmallSet<unsigned, 32> &RegDefs,
                        SmallSet<unsigned, 32> &RegUses);

    bool
    findDelayInstr(MachineBasicBlock &MBB, MachineBasicBlock::iterator slot,
                   MachineBasicBlock::iterator &Filler);
  };
  char Filler::ID = 0;
} // end of anonymous namespace

/// createOR1KDelaySlotFillerPass - Returns a pass that fills in delay
/// slots in OR1K MachineFunctions
///
FunctionPass *llvm::createOR1KDelaySlotFillerPass(OR1KTargetMachine &tm) {
  return new Filler(tm);
}

/// runOnMachineBasicBlock - Fill in delay slots for the given basic block.
/// There is only one delay slot per delayed instruction.
bool Filler::runOnMachineBasicBlock(MachineBasicBlock &MBB) {
  bool Changed = false;
  LastFiller = MBB.end();

  for (MachineBasicBlock::iterator I = MBB.begin(); I != MBB.end(); ++I)
    if (I->getDesc().hasDelaySlot()) {
      MachineBasicBlock::iterator J = I;

      if (!DisableDelaySlotFiller && findDelayInstr(MBB, I, J))
        MBB.splice(llvm::next(I), &MBB, J);
      else
        BuildMI(MBB, llvm::next(I), DebugLoc(), TII->get(OR1K::NOP)).addImm(0);

      ++FilledSlots;
      Changed = true;
      // Record the filler instruction that filled the delay slot.
      // The instruction after it will be visited in the next iteration.
      LastFiller = ++I;
    }
  return Changed;
}

bool Filler::findDelayInstr(MachineBasicBlock &MBB,
                            MachineBasicBlock::iterator slot,
                            MachineBasicBlock::iterator &Filler) {

  SmallSet<unsigned, 32> RegDefs;
  SmallSet<unsigned, 32> RegUses;

  insertDefsUses(slot, RegDefs, RegUses);

  bool sawLoad = false;
  bool sawStore = false;

  for (MachineBasicBlock::reverse_iterator I(slot); I != MBB.rend(); ++I) {
    // skip debug value
    if (I->isDebugValue())
      continue;

    // Convert to forward iterator.
    MachineBasicBlock::iterator FI(llvm::next(I).base());

    if (I->hasUnmodeledSideEffects() ||
        I->isInlineAsm()             ||
        I->isLabel()                 ||
        FI == LastFiller             ||
        I->isPseudo()
        )
      break;

    if (delayHasHazard(FI, sawLoad, sawStore, RegDefs, RegUses)) {
      insertDefsUses(FI, RegDefs, RegUses);
      continue;
    }
    Filler = FI;
    return true;
  }

return false;
}

bool Filler::delayHasHazard(MachineBasicBlock::iterator MI,
                            bool &sawLoad, bool &sawStore,
                            SmallSet<unsigned, 32> &RegDefs,
                            SmallSet<unsigned, 32> &RegUses) {
  if (MI->isImplicitDef() || MI->isKill())
    return true;

  // Loads or stores cannot be moved past a store to the delay slot
  // and stores cannot be moved past a load.
  if (MI->mayLoad()) {
    if (sawStore)
      return true;
    sawLoad = true;
  }

  if (MI->mayStore()) {
    if (sawStore)
      return true;
    sawStore = true;
    if (sawLoad)
      return true;
  }

  assert((!MI->isCall() && !MI->isReturn()) &&
         "Cannot put calls or returns in delay slot.");

  for (unsigned i = 0, e = MI->getNumOperands(); i!= e; ++i) {
    const MachineOperand &MO = MI->getOperand(i);
    unsigned Reg;

    if (!MO.isReg() || !(Reg = MO.getReg()))
      continue; // skip

    if (MO.isDef()) {
      // check whether Reg is defined or used before delay slot.
      if (IsRegInSet(RegDefs, Reg) || IsRegInSet(RegUses, Reg))
        return true;
    }
    if (MO.isUse()) {
      // check whether Reg is defined before delay slot.
      if (IsRegInSet(RegDefs, Reg))
        return true;
    }
  }
  return false;
}

// Insert Defs and Uses of MI into the sets RegDefs and RegUses.
void Filler::insertDefsUses(MachineBasicBlock::iterator MI,
                            SmallSet<unsigned, 32>& RegDefs,
                            SmallSet<unsigned, 32>& RegUses) {
  // If MI is a call or return, just examine the explicit non-variadic operands.
  MCInstrDesc MCID = MI->getDesc();
  unsigned e = MI->isCall() || MI->isReturn() ? MCID.getNumOperands() :
                                                MI->getNumOperands();

  // Add R9 to RegDefs to prevent users of R9 from going into delay slot.
  if (MI->isCall())
    RegDefs.insert(OR1K::R9);

  // Add R9 to RegUses to prevent definers of R9 from going into delay slot.
  if (MI->isReturn())
    RegUses.insert(OR1K::R9);

  for (unsigned i = 0; i != e; ++i) {
    const MachineOperand &MO = MI->getOperand(i);
    unsigned Reg;

    if (!MO.isReg() || !(Reg = MO.getReg()))
      continue;

    if (MO.isDef())
      RegDefs.insert(Reg);
    else if (MO.isUse())
      RegUses.insert(Reg);
  }
}

//returns true if the Reg or its alias is in the RegSet.
bool Filler::IsRegInSet(SmallSet<unsigned, 32>& RegSet, unsigned Reg) {
  if (RegSet.count(Reg))
    return true;
  // check Aliased Registers
  for (const uint16_t *Alias = TM.getRegisterInfo()->getAliasSet(Reg);
       *Alias; ++Alias)
    if (RegSet.count(*Alias))
      return true;

  return false;
}
