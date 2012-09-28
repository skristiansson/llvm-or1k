//===-- OR1KISelLowering.h - OR1K DAG Lowering Interface -......-*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that OR1K uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TARGET_OR1K_ISELLOWERING_H
#define LLVM_TARGET_OR1K_ISELLOWERING_H

#include "OR1K.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/Target/TargetLowering.h"

namespace llvm {
  namespace OR1KISD {
    enum {
      FIRST_NUMBER = ISD::BUILTIN_OP_END,

      ADJDYNALLOC,

      /// Return with a flag operand. Operand 0 is the chain operand.
      RET_FLAG,

      /// CALL - These operations represent an abstract call instruction, which
      /// includes a bunch of information.
      CALL,

      /// SELECT_CC - Operand 0 and operand 1 are selection variable, operand 3
      /// is condition code and operand 4 is flag operand.
      SELECT_CC,

      // SET_FLAG - Set flag compare
      SET_FLAG,

      // BR_CC - Used to glue together a l.bf to a l.sfXX
      BR_CC,

      /// Wrapper - A wrapper node for TargetConstantPool, TargetExternalSymbol,
      /// and TargetGlobalAddress.
      Wrapper,

      /// Find First 1 and Find Last 1 - used to implement CTTZ and CTLZ
      FF1,
      FL1,

      // Get the Higher/Lower 16 bits from a 32-bit immediate
      HI,
      LO
    };
  }

  class OR1KSubtarget;
  class OR1KTargetMachine;

  class OR1KTargetLowering : public TargetLowering {
  public:
    explicit OR1KTargetLowering(OR1KTargetMachine &TM);

    /// LowerOperation - Provide custom lowering hooks for some operations.
    virtual SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const;

    /// getTargetNodeName - This method returns the name of a target specific
    /// DAG node.
    virtual const char *getTargetNodeName(unsigned Opcode) const;

    SDValue LowerBR_CC(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerSELECT_CC(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerVASTART(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerDYNAMIC_STACKALLOC(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerCTTZ(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerCTLZ(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerCTTZ_ZERO_UNDEF(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerConstantPool(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerBlockAddress(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerJumpTable(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerRETURNADDR(SDValue Op, SelectionDAG &DAG) const;
    SDValue LowerFRAMEADDR(SDValue Op, SelectionDAG &DAG) const;

    std::pair<unsigned, const TargetRegisterClass*>
    getRegForInlineAsmConstraint(const std::string &Constraint, EVT VT) const;
    ConstraintWeight
    getSingleConstraintMatchWeight(AsmOperandInfo &info,
                                   const char *constraint) const;
    virtual void LowerAsmOperandForConstraint(SDValue Op,
                                              std::string &Constraint,
                                              std::vector<SDValue> &Ops,
                                              SelectionDAG &DAG) const;

    MachineBasicBlock* EmitInstrWithCustomInserter(MachineInstr *MI,
                                                   MachineBasicBlock *BB) const;

  private:
    const OR1KSubtarget &Subtarget;
    const OR1KTargetMachine &TM;
    const TargetData *TD;

    SDValue LowerCCCCallTo(SDValue Chain, SDValue Callee,
                           CallingConv::ID CallConv, bool isVarArg,
                           bool isTailCall,
                           const SmallVectorImpl<ISD::OutputArg> &Outs,
                           const SmallVectorImpl<SDValue> &OutVals,
                           const SmallVectorImpl<ISD::InputArg> &Ins,
                           DebugLoc dl, SelectionDAG &DAG,
                           SmallVectorImpl<SDValue> &InVals) const;

    SDValue LowerCCCArguments(SDValue Chain,
                              CallingConv::ID CallConv,
                              bool isVarArg,
                              const SmallVectorImpl<ISD::InputArg> &Ins,
                              DebugLoc dl,
                              SelectionDAG &DAG,
                              SmallVectorImpl<SDValue> &InVals) const;

    SDValue LowerCallResult(SDValue Chain, SDValue InFlag,
                            CallingConv::ID CallConv, bool isVarArg,
                            const SmallVectorImpl<ISD::InputArg> &Ins,
                            DebugLoc dl, SelectionDAG &DAG,
                            SmallVectorImpl<SDValue> &InVals) const;

    SDValue LowerCall(TargetLowering::CallLoweringInfo &CLI,
                      SmallVectorImpl<SDValue> &InVals) const;

    SDValue LowerFormalArguments(SDValue Chain,
                                 CallingConv::ID CallConv, bool isVarArg,
                                 const SmallVectorImpl<ISD::InputArg> &Ins,
                                 DebugLoc dl, SelectionDAG &DAG,
                                 SmallVectorImpl<SDValue> &InVals) const;

    SDValue LowerReturn(SDValue Chain,
                        CallingConv::ID CallConv, bool isVarArg,
                        const SmallVectorImpl<ISD::OutputArg> &Outs,
                        const SmallVectorImpl<SDValue> &OutVals,
                        DebugLoc dl, SelectionDAG &DAG) const;
  };
} // namespace llvm

#endif // LLVM_TARGET_OPENRISC_ISELLOWERING_H
