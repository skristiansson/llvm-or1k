//===-- OR1KISelLowering.cpp - OR1K DAG Lowering Implementation -------===//
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

#define DEBUG_TYPE "or1k-lower"
#include "OR1KISelLowering.h"
#include "OR1KMachineFunctionInfo.h"
#include "OR1KTargetMachine.h"
#include "OR1KSubtarget.h"
#include "llvm/DerivedTypes.h"
#include "llvm/Function.h"
#include "llvm/GlobalVariable.h"
#include "llvm/Intrinsics.h"
#include "llvm/CallingConv.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

const char *OR1KTargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch (Opcode) {
    case OR1KISD::JmpLink    : return "OR1KISD::JmpLink";
    case OR1KISD::GPRel      : return "OR1KISD::GPRel";
    case OR1KISD::Wrap       : return "OR1KISD::Wrap";
    case OR1KISD::ICmp       : return "OR1KISD::ICmp";
    case OR1KISD::Ret        : return "OR1KISD::Ret";
    case OR1KISD::Select_CC  : return "OR1KISD::Select_CC";
    default                    : return NULL;
  }
}

OR1KTargetLowering::OR1KTargetLowering(OR1KTargetMachine &TM)
  : TargetLowering(TM, new TargetLoweringObjectFileELF()) {

  // OR1K does not have i1 type, so use i32 for
  // setcc operations results (slt, sgt, ...).
  setBooleanContents(ZeroOrOneBooleanContent);

  // Set up the register classes
  addRegisterClass(MVT::i32, OR1K::GPRRegisterClass);

  // Floating point operations which are not supported
  setOperationAction(ISD::FREM,       MVT::f32, Expand);
  setOperationAction(ISD::UINT_TO_FP, MVT::i8,  Expand);
  setOperationAction(ISD::UINT_TO_FP, MVT::i16, Expand);
  setOperationAction(ISD::UINT_TO_FP, MVT::i32, Expand);
  setOperationAction(ISD::FP_TO_UINT, MVT::i32, Expand);
  setOperationAction(ISD::FP_ROUND,   MVT::f32, Expand);
  setOperationAction(ISD::FP_ROUND,   MVT::f64, Expand);
  setOperationAction(ISD::FCOPYSIGN,  MVT::f32, Expand);
  setOperationAction(ISD::FCOPYSIGN,  MVT::f64, Expand);
  setOperationAction(ISD::FSIN,       MVT::f32, Expand);
  setOperationAction(ISD::FCOS,       MVT::f32, Expand);
  setOperationAction(ISD::FPOWI,      MVT::f32, Expand);
  setOperationAction(ISD::FPOW,       MVT::f32, Expand);
  setOperationAction(ISD::FLOG,       MVT::f32, Expand);
  setOperationAction(ISD::FLOG2,      MVT::f32, Expand);
  setOperationAction(ISD::FLOG10,     MVT::f32, Expand);
  setOperationAction(ISD::FEXP,       MVT::f32, Expand);

  // Load extented operations for i1 types must be promoted
  setLoadExtAction(ISD::EXTLOAD,  MVT::i1,  Promote);
  setLoadExtAction(ISD::ZEXTLOAD, MVT::i1,  Promote);
  setLoadExtAction(ISD::SEXTLOAD, MVT::i1,  Promote);

  // OR1K has no REM or DIVREM operations.
  setOperationAction(ISD::UREM,    MVT::i32, Expand);
  setOperationAction(ISD::SREM,    MVT::i32, Expand);
  setOperationAction(ISD::SDIVREM, MVT::i32, Expand);
  setOperationAction(ISD::UDIVREM, MVT::i32, Expand);

  // Expand unsupported conversions
  setOperationAction(ISD::BITCAST, MVT::f32, Expand);
  setOperationAction(ISD::BITCAST, MVT::i32, Expand);

  // Expand SELECT_CC
  setOperationAction(ISD::SELECT_CC, MVT::Other, Expand);

  // OR1K doesn't have MUL_LOHI
  setOperationAction(ISD::SMUL_LOHI, MVT::i32, Expand);
  setOperationAction(ISD::UMUL_LOHI, MVT::i32, Expand);
  setOperationAction(ISD::SMUL_LOHI, MVT::i64, Expand);
  setOperationAction(ISD::UMUL_LOHI, MVT::i64, Expand);

  // Used by legalize types to correctly generate the setcc result.
  // Without this, every float setcc comes with a AND/OR with the result,
  // we don't want this, since the fpcmp result goes to a flag register,
  // which is used implicitly by brcond and select operations.
  AddPromotedToType(ISD::SETCC, MVT::i1, MVT::i32);
  AddPromotedToType(ISD::SELECT, MVT::i1, MVT::i32);
  AddPromotedToType(ISD::SELECT_CC, MVT::i1, MVT::i32);

  // OR1K Custom Operations
  setOperationAction(ISD::GlobalAddress,      MVT::i32,   Custom);
  setOperationAction(ISD::GlobalTLSAddress,   MVT::i32,   Custom);
  setOperationAction(ISD::JumpTable,          MVT::i32,   Custom);
  setOperationAction(ISD::ConstantPool,       MVT::i32,   Custom);

  // Variable Argument support
  setOperationAction(ISD::VASTART,            MVT::Other, Custom);
  setOperationAction(ISD::VAEND,              MVT::Other, Expand);
  setOperationAction(ISD::VAARG,              MVT::Other, Expand);
  setOperationAction(ISD::VACOPY,             MVT::Other, Expand);


  // Operations not directly supported by OR1K.
  setOperationAction(ISD::DYNAMIC_STACKALLOC, MVT::i32,   Expand);
  setOperationAction(ISD::BR_JT,              MVT::Other, Expand);
  setOperationAction(ISD::BR_CC,              MVT::Other, Expand);
  setOperationAction(ISD::SIGN_EXTEND_INREG,  MVT::i1,    Expand);
  setOperationAction(ISD::ROTL,               MVT::i32,   Expand);
  setOperationAction(ISD::ROTR,               MVT::i32,   Expand);
  setOperationAction(ISD::SHL_PARTS,          MVT::i32,   Expand);
  setOperationAction(ISD::SRA_PARTS,          MVT::i32,   Expand);
  setOperationAction(ISD::SRL_PARTS,          MVT::i32,   Expand);
  setOperationAction(ISD::CTLZ,               MVT::i32,   Expand);
  setOperationAction(ISD::CTTZ,               MVT::i32,   Expand);
  setOperationAction(ISD::CTPOP,              MVT::i32,   Expand);
  setOperationAction(ISD::BSWAP,              MVT::i32,   Expand);

  // We don't have line number support yet.
  setOperationAction(ISD::EH_LABEL,          MVT::Other, Expand);

  // Use the default for now
  setOperationAction(ISD::STACKSAVE,         MVT::Other, Expand);
  setOperationAction(ISD::STACKRESTORE,      MVT::Other, Expand);
  setOperationAction(ISD::MEMBARRIER,        MVT::Other, Expand);

  // OR1K doesn't have extending float->double load/store
  setLoadExtAction(ISD::EXTLOAD, MVT::f32, Expand);
  setTruncStoreAction(MVT::f64, MVT::f32, Expand);

  setStackPointerRegisterToSaveRestore(OR1K::R1);
  computeRegisterProperties();
}

EVT OR1KTargetLowering::getSetCCResultType(EVT VT) const {
  return MVT::i32;
}

/// getFunctionAlignment - Return the Log2 alignment of this function.
unsigned OR1KTargetLowering::getFunctionAlignment(const Function *) const {
  return 2;
}

SDValue OR1KTargetLowering::LowerOperation(SDValue Op,
                                             SelectionDAG &DAG) const {
/*  switch (Op.getOpcode())
  {
    case ISD::ConstantPool:       return LowerConstantPool(Op, DAG);
    case ISD::GlobalAddress:      return LowerGlobalAddress(Op, DAG);
    case ISD::GlobalTLSAddress:   return LowerGlobalTLSAddress(Op, DAG);
    case ISD::JumpTable:          return LowerJumpTable(Op, DAG);
    case ISD::SELECT_CC:          return LowerSELECT_CC(Op, DAG);
    case ISD::VASTART:            return LowerVASTART(Op, DAG);
  }
*/
  return SDValue();
}

//===----------------------------------------------------------------------===//
//  Lower helper functions
//===----------------------------------------------------------------------===//
/*
MachineBasicBlock*
OR1KTargetLowering::EmitInstrWithCustomInserter(MachineInstr *MI,
                                                  MachineBasicBlock *BB) const {
  const TargetInstrInfo *TII = getTargetMachine().getInstrInfo();
  DebugLoc dl = MI->getDebugLoc();

  switch (MI->getOpcode()) {
  default: assert(false && "Unexpected instr type to insert");
  }
}
*/
//===----------------------------------------------------------------------===//
//  Misc Lower Operation implementation
//===----------------------------------------------------------------------===//
//

SDValue OR1KTargetLowering::LowerSELECT_CC(SDValue Op,
                                             SelectionDAG &DAG) const {
  SDValue LHS = Op.getOperand(0);
  SDValue RHS = Op.getOperand(1);
  SDValue TrueVal = Op.getOperand(2);
  SDValue FalseVal = Op.getOperand(3);
  DebugLoc dl = Op.getDebugLoc();
  unsigned Opc;

  SDValue CompareFlag;
  if (LHS.getValueType() == MVT::i32) {
    Opc = OR1KISD::Select_CC;
    CompareFlag = DAG.getNode(OR1KISD::ICmp, dl, MVT::i32, LHS, RHS)
                    .getValue(1);
  } else {
    llvm_unreachable( "Cannot lower select_cc with unknown type" );
  }
 
  return DAG.getNode(Opc, dl, TrueVal.getValueType(), TrueVal, FalseVal,
                     CompareFlag);
}

SDValue OR1KTargetLowering::
LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const {
  // FIXME there isn't actually debug info here
  DebugLoc dl = Op.getDebugLoc();
  const GlobalValue *GV = cast<GlobalAddressSDNode>(Op)->getGlobal();
  SDValue GA = DAG.getTargetGlobalAddress(GV, dl, MVT::i32);

  return DAG.getNode(OR1KISD::Wrap, dl, MVT::i32, GA);
}

SDValue OR1KTargetLowering::
LowerGlobalTLSAddress(SDValue Op, SelectionDAG &DAG) const {
  llvm_unreachable("TLS not implemented for MicroBlaze.");
  return SDValue(); // Not reached
}


SDValue OR1KTargetLowering::
LowerJumpTable(SDValue Op, SelectionDAG &DAG) const {
  return SDValue();
#if 0
  SDValue JTI = DAG.getTargetJumpTable(JT->getIndex(), PtrVT, OpFlag);
  return JTI;
#endif
}

SDValue OR1KTargetLowering::
LowerConstantPool(SDValue Op, SelectionDAG &DAG) const {
  return SDValue();
#if 0
  SDValue ResNode;
  ConstantPoolSDNode *N = cast<ConstantPoolSDNode>(Op);
  const Constant *C = N->getConstVal();
  DebugLoc dl = Op.getDebugLoc();

  SDValue CP = DAG.getTargetConstantPool(C, MVT::i32, N->getAlignment(),
                                         N->getOffset(), OR1KII::MO_ABS_HILO);
  return DAG.getNode(OR1KISD::Wrap, dl, MVT::i32, CP);
#endif
}


//===----------------------------------------------------------------------===//
//                      Calling Convention Implementation
//===----------------------------------------------------------------------===//

#include "OR1KGenCallingConv.inc"

static bool CC_OR1K2(unsigned ValNo, EVT ValVT,
                       EVT LocVT, CCValAssign::LocInfo LocInfo,
                       ISD::ArgFlagsTy ArgFlags, CCState &State) {
  return false; // CC must always match
}

//===----------------------------------------------------------------------===//
//                  Call Calling Convention Implementation
//===----------------------------------------------------------------------===//

/// LowerCall - functions arguments are copied from virtual regs to
/// (physical regs)/(stack frame), CALLSEQ_START and CALLSEQ_END are emitted.
/// TODO: isVarArg, isTailCall.

SDValue OR1KTargetLowering::
LowerCall(SDValue Chain, SDValue Callee, CallingConv::ID CallConv,
          bool isVarArg, bool doesNotRet, bool &isTailCall,
          const SmallVectorImpl<ISD::OutputArg> &Outs,
          const SmallVectorImpl<SDValue> &OutVals,
          const SmallVectorImpl<ISD::InputArg> &Ins,
          DebugLoc dl, SelectionDAG &DAG,
          SmallVectorImpl<SDValue> &InVals) const {


  return SDValue();
#if 0

  // OR1K does not yet support tail call optimization
  isTailCall = false;

  MachineFunction &MF = DAG.getMachineFunction();
  MachineFrameInfo *MFI = MF.getFrameInfo();

  // Analyze operands of the call, assigning locations to each operand.
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, isVarArg, getTargetMachine(), ArgLocs,
                 *DAG.getContext());
  CCInfo.AnalyzeCallOperands(Outs, CC_OR1K2);

  // Get a count of how many bytes are to be pushed on the stack.
  unsigned NumBytes = CCInfo.getNextStackOffset();
  Chain = DAG.getCALLSEQ_START(Chain, DAG.getIntPtrConstant(NumBytes, true));

  SmallVector<std::pair<unsigned, SDValue>, 8> RegsToPass;
  SmallVector<SDValue, 8> MemOpChains;

  // First/LastArgStackLoc contains the first/last
  // "at stack" argument location.
  int LastArgStackLoc = 0;
  unsigned FirstStackArgLoc = 0;

  // Walk the register/memloc assignments, inserting copies/loads.
  for (unsigned i = 0, e = ArgLocs.size(); i != e; ++i) {
    CCValAssign &VA = ArgLocs[i];
    MVT RegVT = VA.getLocVT();
    SDValue Arg = OutVals[i];

    // Promote the value if needed.
    switch (VA.getLocInfo()) {
    default: llvm_unreachable("Unknown loc info!");
    case CCValAssign::Full: break;
    case CCValAssign::SExt:
      Arg = DAG.getNode(ISD::SIGN_EXTEND, dl, RegVT, Arg);
      break;
    case CCValAssign::ZExt:
      Arg = DAG.getNode(ISD::ZERO_EXTEND, dl, RegVT, Arg);
      break;
    case CCValAssign::AExt:
      Arg = DAG.getNode(ISD::ANY_EXTEND, dl, RegVT, Arg);
      break;
    }

    // Arguments that can be passed on register must be kept at
    // RegsToPass vector
    if (VA.isRegLoc()) {
      RegsToPass.push_back(std::make_pair(VA.getLocReg(), Arg));
    } else {
      // Register can't get to this point...
      assert(VA.isMemLoc());

      // Create the frame index object for this incoming parameter
      LastArgStackLoc = (FirstStackArgLoc + VA.getLocMemOffset());
      int FI = MFI->CreateFixedObject(VA.getValVT().getSizeInBits()/8,
                                      LastArgStackLoc, true);

      SDValue PtrOff = DAG.getFrameIndex(FI,getPointerTy());

      // emit ISD::STORE whichs stores the
      // parameter value to a stack Location
      MemOpChains.push_back(DAG.getStore(Chain, dl, Arg, PtrOff, NULL, 0,
                                         false, false, 0));
    }
  }

  // Transform all store nodes into one single node because all store
  // nodes are independent of each other.
  if (!MemOpChains.empty())
    Chain = DAG.getNode(ISD::TokenFactor, dl, MVT::Other,
                        &MemOpChains[0], MemOpChains.size());

  // Build a sequence of copy-to-reg nodes chained together with token
  // chain and flag operands which copy the outgoing args into registers.
  // The InFlag in necessary since all emited instructions must be
  // stuck together.
  SDValue InFlag;
  for (unsigned i = 0, e = RegsToPass.size(); i != e; ++i) {
    Chain = DAG.getCopyToReg(Chain, dl, RegsToPass[i].first,
                             RegsToPass[i].second, InFlag);
    InFlag = Chain.getValue(1);
  }

  // If the callee is a GlobalAddress/ExternalSymbol node (quite common, every
  // direct call is) turn it into a TargetGlobalAddress/TargetExternalSymbol
  // node so that legalize doesn't hack it.
  unsigned char OpFlag = OR1KII::MO_NO_FLAG;
  if (GlobalAddressSDNode *G = dyn_cast<GlobalAddressSDNode>(Callee))
    Callee = DAG.getTargetGlobalAddress(G->getGlobal(), dl,
                                getPointerTy(), 0, OpFlag);
  else if (ExternalSymbolSDNode *S = dyn_cast<ExternalSymbolSDNode>(Callee))
    Callee = DAG.getTargetExternalSymbol(S->getSymbol(),
                                getPointerTy(), OpFlag);

  // OR1KJmpLink = #chain, #target_address, #opt_in_flags...
  //             = Chain, Callee, Reg#1, Reg#2, ...
  //
  // Returns a chain & a flag for retval copy to use.
  SDVTList NodeTys = DAG.getVTList(MVT::Other, MVT::Flag);
  SmallVector<SDValue, 8> Ops;
  Ops.push_back(Chain);
  Ops.push_back(Callee);

  // Add argument registers to the end of the list so that they are
  // known live into the call.
  for (unsigned i = 0, e = RegsToPass.size(); i != e; ++i) {
    Ops.push_back(DAG.getRegister(RegsToPass[i].first,
                                  RegsToPass[i].second.getValueType()));
  }

  if (InFlag.getNode())
    Ops.push_back(InFlag);

  Chain  = DAG.getNode(OR1KISD::JmpLink, dl, NodeTys, &Ops[0], Ops.size());
  InFlag = Chain.getValue(1);

  // Create the CALLSEQ_END node.
  Chain = DAG.getCALLSEQ_END(Chain, DAG.getIntPtrConstant(NumBytes, true),
                             DAG.getIntPtrConstant(0, true), InFlag);
  if (!Ins.empty())
    InFlag = Chain.getValue(1);

  // Handle result values, copying them out of physregs into vregs that we
  // return.
  return LowerCallResult(Chain, InFlag, CallConv, isVarArg,
                         Ins, dl, DAG, InVals);

#endif
}

/// LowerCallResult - Lower the result values of a call into the
/// appropriate copies out of appropriate physical registers.
/*
SDValue OR1KTargetLowering::
LowerCallResult(SDValue Chain, SDValue InFlag, CallingConv::ID CallConv,
                bool isVarArg, const SmallVectorImpl<ISD::InputArg> &Ins,
                DebugLoc dl, SelectionDAG &DAG,
                SmallVectorImpl<SDValue> &InVals) const {
  // Assign locations to each value returned by this call.
  SmallVector<CCValAssign, 16> RVLocs;
  CCState CCInfo(CallConv, isVarArg, getTargetMachine(),
                 RVLocs, *DAG.getContext());

  CCInfo.AnalyzeCallResult(Ins, RetCC_OR1K);

  // Copy all of the result registers out of their specified physreg.
  for (unsigned i = 0; i != RVLocs.size(); ++i) {
    Chain = DAG.getCopyFromReg(Chain, dl, RVLocs[i].getLocReg(),
                               RVLocs[i].getValVT(), InFlag).getValue(1);
    InFlag = Chain.getValue(2);
    InVals.push_back(Chain.getValue(0));
  } 

  return Chain;
}
*/
//===----------------------------------------------------------------------===//
//             Formal Arguments Calling Convention Implementation
//===----------------------------------------------------------------------===//

/// LowerFormalArguments - transform physical registers into
/// virtual registers and generate load operations for
/// arguments places on the stack.
SDValue OR1KTargetLowering::
LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
                     const SmallVectorImpl<ISD::InputArg> &Ins,
                     DebugLoc dl, SelectionDAG &DAG,
                     SmallVectorImpl<SDValue> &InVals) const {

	return Chain;
}

//===----------------------------------------------------------------------===//
//               Return Value Calling Convention Implementation
//===----------------------------------------------------------------------===//

SDValue OR1KTargetLowering::
LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
            const SmallVectorImpl<ISD::OutputArg> &Outs,
            const SmallVectorImpl<SDValue> &OutVals,
            DebugLoc dl, SelectionDAG &DAG) const {

    return DAG.getNode(OR1KISD::Ret, dl, MVT::Other,
                       Chain, DAG.getRegister(OR1K::R15, MVT::i32));
#if 0
  // CCValAssign - represent the assignment of
  // the return value to a location
  SmallVector<CCValAssign, 16> RVLocs;

  // CCState - Info about the registers and stack slot.
  CCState CCInfo(CallConv, isVarArg, getTargetMachine(),
                 RVLocs, *DAG.getContext());

  // Analize return values.
  CCInfo.AnalyzeReturn(Outs, RetCC_OR1K);

  // If this is the first return lowered for this function, add
  // the regs to the liveout set for the function.
  if (DAG.getMachineFunction().getRegInfo().liveout_empty()) {
    for (unsigned i = 0; i != RVLocs.size(); ++i)
      if (RVLocs[i].isRegLoc())
        DAG.getMachineFunction().getRegInfo().addLiveOut(RVLocs[i].getLocReg());
  }

  SDValue Flag;

  // Copy the result values into the output registers.
  for (unsigned i = 0; i != RVLocs.size(); ++i) {
    CCValAssign &VA = RVLocs[i];
    assert(VA.isRegLoc() && "Can only return in registers!");

    Chain = DAG.getCopyToReg(Chain, dl, VA.getLocReg(),
                             OutVals[i], Flag);

    // guarantee that all emitted copies are
    // stuck together, avoiding something bad
    Flag = Chain.getValue(1);
  }

  // Return on OR1K is always a "rtsd R15, 8"
  if (Flag.getNode())
    return DAG.getNode(OR1KISD::Ret, dl, MVT::Other,
                       Chain, DAG.getRegister(OR1K::R15, MVT::i32), Flag);
  else // Return Void
    return DAG.getNode(OR1KISD::Ret, dl, MVT::Other,
                       Chain, DAG.getRegister(OR1K::R15, MVT::i32));
#endif
}

//===----------------------------------------------------------------------===//
//                           OR1K Inline Assembly Support
//===----------------------------------------------------------------------===//

/// getConstraintType - Given a constraint letter, return the type of
/// constraint it is for this target.
OR1KTargetLowering::ConstraintType OR1KTargetLowering::
getConstraintType(const std::string &Constraint) const
{
  // OR1K specific constrainy
  //
  // 'd' : An address register. Equivalent to r.
  // 'y' : Equivalent to r; retained for
  //       backwards compatibility.
  // 'f' : Floating Point registers.
  if (Constraint.size() == 1) {
    switch (Constraint[0]) {
      default : break;
      case 'd':
      case 'y':
      case 'f':
        return C_RegisterClass;
        break;
    }
  }
  return TargetLowering::getConstraintType(Constraint);
}

/// getRegClassForInlineAsmConstraint - Given a constraint letter (e.g. "r"),
/// return a list of registers that can be used to satisfy the constraint.
/// This should only be used for C_RegisterClass constraints.
std::pair<unsigned, const TargetRegisterClass*> OR1KTargetLowering::
getRegForInlineAsmConstraint(const std::string &Constraint, EVT VT) const {
  return TargetLowering::getRegForInlineAsmConstraint(Constraint, VT);
}

/// Given a register class constraint, like 'r', if this corresponds directly
/// to an LLVM register class, return a register of 0 and the register class
/// pointer.
std::vector<unsigned> OR1KTargetLowering::
getRegClassForInlineAsmConstraint(const std::string &Constraint, EVT VT) const {
    return std::vector<unsigned>();
}

bool OR1KTargetLowering::
isOffsetFoldingLegal(const GlobalAddressSDNode *GA) const {
  // The OR1K target isn't yet aware of offsets.
  return false;
}

bool OR1KTargetLowering::isFPImmLegal(const APFloat &Imm, EVT VT) const {
  return VT != MVT::f32;
}
