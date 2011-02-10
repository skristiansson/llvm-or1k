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

#ifndef MBLAZE_TARGETMACHINE_H
#define MBLAZE_TARGETMACHINE_H

#include "OR1KSubtarget.h"
#include "OR1KInstrInfo.h"
#include "OR1KISelLowering.h"
#include "OR1KSelectionDAGInfo.h"
#include "OR1KIntrinsicInfo.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetData.h"
#include "llvm/Target/TargetFrameInfo.h"

namespace llvm {
  class formatted_raw_ostream;

  class OR1KTargetMachine : public LLVMTargetMachine {
    OR1KSubtarget       Subtarget;
    const TargetData    DataLayout; // Calculates type size & alignment
    OR1KInstrInfo       InstrInfo;
    TargetFrameInfo     FrameInfo;
    OR1KTargetLowering  TLInfo;
    OR1KSelectionDAGInfo TSInfo;
    OR1KIntrinsicInfo IntrinsicInfo;
  public:
    OR1KTargetMachine(const Target &T, const std::string &TT,
                      const std::string &FS);

    virtual const OR1KInstrInfo *getInstrInfo() const
    { return &InstrInfo; }

    virtual const TargetFrameInfo *getFrameInfo() const
    { return &FrameInfo; }

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

    const TargetIntrinsicInfo *getIntrinsicInfo() const
    { return &IntrinsicInfo; }

    // Pass Pipeline Configuration
    virtual bool addInstSelector(PassManagerBase &PM,
                                 CodeGenOpt::Level OptLevel);

    virtual bool addPreEmitPass(PassManagerBase &PM,
                                CodeGenOpt::Level OptLevel);
  };
} // End llvm namespace

#endif
