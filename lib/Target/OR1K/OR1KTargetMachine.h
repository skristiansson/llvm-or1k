//===-- OR1KTargetMachine.h - Define TargetMachine for OR1K --- C++ ---===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the OR1K specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef OR1K_TARGETMACHINE_H
#define OR1K_TARGETMACHINE_H

#include "OR1KSubtarget.h"
#include "OR1KInstrInfo.h"
#include "OR1KISelLowering.h"
#include "OR1KSelectionDAGInfo.h"
//#include "OR1KIntrinsicInfo.h"
#include "OR1KFrameLowering.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetData.h"
#include "llvm/Target/TargetFrameLowering.h"

namespace llvm {
  class formatted_raw_ostream;

  class OR1KTargetMachine : public LLVMTargetMachine {
    OR1KSubtarget       Subtarget;
    const TargetData    DataLayout; // Calculates type size & alignment
    OR1KInstrInfo       InstrInfo;
    OR1KTargetLowering  TLInfo;
    OR1KSelectionDAGInfo TSInfo;
    OR1KFrameLowering   FrameLowering;
/*    OR1KIntrinsicInfo IntrinsicInfo;*/
  public:
    OR1KTargetMachine(const Target &T, StringRef TT,
                        StringRef CPU, StringRef FS,
                        const TargetOptions &Options,
                        Reloc::Model RM, CodeModel::Model CM,
                        CodeGenOpt::Level OL);

    virtual const OR1KInstrInfo *getInstrInfo() const
    { return &InstrInfo; }

    virtual const TargetFrameLowering *getFrameLowering() const
    { return &FrameLowering; }

    virtual const OR1KSubtarget *getSubtargetImpl() const
    { return &Subtarget; }

    virtual const TargetData *getTargetData() const
    { return &DataLayout;}

    virtual const OR1KRegisterInfo *getRegisterInfo() const
    { return &InstrInfo.getRegisterInfo(); }

    virtual const OR1KTargetLowering *getTargetLowering() const
    { return &TLInfo; }

    virtual const OR1KSelectionDAGInfo* getSelectionDAGInfo() const
    { return &TSInfo; }

/*    const TargetIntrinsicInfo *getIntrinsicInfo() const
    { return &IntrinsicInfo; }
*/
    // Pass Pipeline Configuration
    virtual TargetPassConfig *createPassConfig(PassManagerBase &PM);
  };
} // End llvm namespace

#endif
