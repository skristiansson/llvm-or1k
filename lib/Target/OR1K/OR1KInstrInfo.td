//===-- OR1KInstrInfo.td - Target Description for OR1K Target -------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes the OR1K instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Instruction format superclass
//===----------------------------------------------------------------------===//

include "OR1KInstrFormats.td"

// -------------------------------------------------- //
// Instruction Operands and Patterns
// -------------------------------------------------- //

//  These are target-independent nodes, but have target-specific formats.
def SDT_OR1KCallSeqStart : SDCallSeqStart<[ SDTCisVT<0, i32> ]>;
def SDT_OR1KCallSeqEnd   : SDCallSeqEnd<[ SDTCisVT<0, i32>,
                                          SDTCisVT<1, i32> ]>;
def SDT_OR1KCall         : SDTypeProfile<0, -1, [SDTCisVT<0, i32>]>;
def SDT_OR1KSetFlag      : SDTypeProfile<0, 3, [SDTCisSameAs<0, 1>]>;
def SDT_OR1KSelectCC     : SDTypeProfile<1, 2, [SDTCisSameAs<0, 1>,
                                                SDTCisSameAs<1, 2>]>;
def SDT_OR1KBrCC         : SDTypeProfile<0, 2, [SDTCisVT<0, OtherVT>]>;
def SDT_OR1KWrapper      : SDTypeProfile<1, 1, [SDTCisSameAs<0, 1>,
                                                SDTCisPtrTy<0>]>;
def SDT_OR1KAdjDynAlloc  : SDTypeProfile<1, 1, [SDTCisVT<0, i32>,
                                                SDTCisVT<1, i32>]>;

def call            : SDNode<"OR1KISD::CALL", SDT_OR1KCall,
                             [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue,
                              SDNPVariadic]>;
def retflag         : SDNode<"OR1KISD::RET_FLAG", SDTNone,
                             [SDNPHasChain, SDNPOptInGlue]>;
def callseq_start   : SDNode<"ISD::CALLSEQ_START", SDT_OR1KCallSeqStart,
                             [SDNPHasChain, SDNPOutGlue]>;
def callseq_end     : SDNode<"ISD::CALLSEQ_END",   SDT_OR1KCallSeqEnd,
                             [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;
def OR1Ksetflag     : SDNode<"OR1KISD::SET_FLAG", SDT_OR1KSetFlag,
                             [SDNPOutGlue]>;
def OR1Kbrcc        : SDNode<"OR1KISD::BR_CC", SDT_OR1KBrCC,
                              [SDNPHasChain, SDNPInGlue]>;
def OR1Kselectcc    : SDNode<"OR1KISD::SELECT_CC", SDT_OR1KSelectCC,
                              [SDNPInGlue]>;
def OR1KWrapper     : SDNode<"OR1KISD::Wrapper", SDT_OR1KWrapper>;
def OR1KHi          : SDNode<"OR1KISD::HI", SDTIntUnaryOp>;
def OR1KLo          : SDNode<"OR1KISD::LO", SDTIntUnaryOp>;
def OR1Kadjdynalloc : SDNode<"OR1KISD::ADJDYNALLOC", SDT_OR1KAdjDynAlloc>;
def OR1Kff1         : SDNode<"OR1KISD::FF1", SDTIntUnaryOp>;
def OR1Kfl1         : SDNode<"OR1KISD::FL1", SDTIntUnaryOp>;

// Extract bits 0-15 (low-end) of an immediate value.
def LO16 : SDNodeXForm<imm, [{
  return CurDAG->getTargetConstant((uint64_t)N->getZExtValue() & 0xffff,
                                   MVT::i32);
}]>;

// Extract bits 16-31 (high-end) of an immediate value.
// Transformation function: shift the immediate value down into the low bits.
def HI16 : SDNodeXForm<imm, [{
  return CurDAG->getTargetConstant((uint64_t)N->getZExtValue() >> 16, MVT::i32);
}]>;


def brtarget : Operand<OtherVT>;
def calltarget : Operand<i32>;

def s6imm    : Operand<i32>;

def s16imm   : Operand<i32> {
  let PrintMethod = "printS16ImmOperand";
}

def immZExt6  : PatLeaf<(imm),
                [{return isInt<6>(N->getZExtValue()); }]>;

def immSExt16 : PatLeaf<(imm),
                [{return isInt<16>(N->getSExtValue()); }]>;

def immZExt16 : PatLeaf<(imm),
                [{return isUInt<16>(N->getZExtValue()); }], LO16>;

def i32lo16 : PatLeaf<(i32 imm), [{
  // i32lo16 predicate - true if the 32-bit immediate has only rightmost 16
  // bits set.
  return ((N->getZExtValue() & 0xFFFFULL) == N->getZExtValue());
}], LO16>;

def i32hi16 : PatLeaf<(i32 imm), [{
  // i32hi16 predicate - true if the 32-bit immediate has only leftmost 16
  // bits set.
  return ((N->getZExtValue() & 0xFFFF0000ULL) == N->getZExtValue());
}], HI16>;

// Addressing modes.
def ADDRri : ComplexPattern<i32, 2, "SelectAddr", [frameindex], []>;

// Address operands
def MEMri : Operand<i32> {
  let PrintMethod = "printMemOperand";
  let EncoderMethod = "getMemoryOpValue";
  let DecoderMethod = "DecodeMemoryValue";
  let MIOperandInfo = (ops GPR, i32imm);
}

// Conditional code predicates - used for pattern matching for SF instructions
def OR1K_CC_EQ  : PatLeaf<(imm),
                  [{return (N->getZExtValue() == ISD::SETEQ);}]>;
def OR1K_CC_NE  : PatLeaf<(imm),
                  [{return (N->getZExtValue() == ISD::SETNE);}]>;
def OR1K_CC_LE  : PatLeaf<(imm),
                  [{return (N->getZExtValue() == ISD::SETLE);}]>;
def OR1K_CC_LT  : PatLeaf<(imm),
                  [{return (N->getZExtValue() == ISD::SETLT);}]>;
def OR1K_CC_GE  : PatLeaf<(imm),
                  [{return (N->getZExtValue() == ISD::SETGE);}]>;
def OR1K_CC_GT  : PatLeaf<(imm),
                  [{return (N->getZExtValue() == ISD::SETGT);}]>;
def OR1K_CC_GTU : PatLeaf<(imm),
                  [{return (N->getZExtValue() == ISD::SETUGT);}]>;
def OR1K_CC_GEU : PatLeaf<(imm),
                  [{return (N->getZExtValue() == ISD::SETUGE);}]>;
def OR1K_CC_LTU : PatLeaf<(imm),
                  [{return (N->getZExtValue() == ISD::SETULT);}]>;
def OR1K_CC_LEU : PatLeaf<(imm),
                  [{return (N->getZExtValue() == ISD::SETULE);}]>;
def OR1K_CC_GTO : PatLeaf<(imm),
                  [{return (N->getZExtValue() == ISD::SETOGT);}]>;
def OR1K_CC_GEO : PatLeaf<(imm),
                  [{return (N->getZExtValue() == ISD::SETOGE);}]>;
def OR1K_CC_LTO : PatLeaf<(imm),
                  [{return (N->getZExtValue() == ISD::SETOLT);}]>;
def OR1K_CC_LEO : PatLeaf<(imm),
                  [{return (N->getZExtValue() == ISD::SETOLE);}]>;
def OR1K_CC_EQO : PatLeaf<(imm),
                  [{return (N->getZExtValue() == ISD::SETOEQ);}]>;
def OR1K_CC_NEO : PatLeaf<(imm),
                  [{return (N->getZExtValue() == ISD::SETONE);}]>;
def OR1K_CC_O   : PatLeaf<(imm),
                  [{return (N->getZExtValue() == ISD::SETO);}]>;
def OR1K_CC_UO  : PatLeaf<(imm),
                  [{return (N->getZExtValue() == ISD::SETUO);}]>;

def OR1K_COND     : PatLeaf<(i32 0)>;
def OR1K_COND_NEG : PatLeaf<(i32 1)>;

// Feature support predicates
def HasDiv       : Predicate<"Subtarget.hasDiv()">,
                   AssemblerPredicate<"FeatureDiv">;
def HasMul       : Predicate<"Subtarget.hasMul()">,
                   AssemblerPredicate<"FeatureMul">;
def HasRor       : Predicate<"Subtarget.hasRor()">,
                   AssemblerPredicate<"FeatureRor">;
def HasCmov      : Predicate<"Subtarget.hasCmov()">,
                   AssemblerPredicate<"FeatureCmov">;
def NoCmov       : Predicate<"!Subtarget.hasCmov()">,
                   AssemblerPredicate<"!FeatureCmov">;


// -------------------------------------------------- //
// Condition/SF instructions
// -------------------------------------------------- //

class SF_RR<bits<5> op2Val, string asmstr, PatLeaf Cond>
  : InstRR<0x9, (outs), (ins GPR:$rA, GPR:$rB),
           !strconcat(asmstr, "\t$rA, $rB"),
           [(OR1Ksetflag (i32 GPR:$rA), (i32 GPR:$rB), Cond)]> {
  bits<5> op2;
  bits<5> rA;
  bits<5> rB;

  let Inst{25-21} = op2;
  let Inst{20-16} = rA;
  let Inst{15-11} = rB;

  let op2 = op2Val;
}

class SF_RI<bits<5> op2Val, string asmstr, PatLeaf Cond>
  : InstRI<0xf, (outs), (ins GPR:$rA, s16imm:$imm),
           !strconcat(asmstr, "i\t$rA, $imm"),
           [(OR1Ksetflag (i32 GPR:$rA), immSExt16:$imm, Cond)]> {
  bits<5> op2;
  bits<5> rA;
  bits<16> imm;

  let Inst{25-21} = op2;
  let Inst{20-16} = rA;
  let Inst{15-0} = imm;

  let op2 = op2Val;
}

multiclass SF<bits<5> op2Val, string asmstr, PatLeaf Cond> {
  def _rr : SF_RR<op2Val, asmstr, Cond>;
  def _ri : SF_RI<op2Val, asmstr, Cond>;
}

// l.sfxx instructions
let Defs = [SR], hasSideEffects = 1 in {
defm SFEQ  : SF<0x0, "l.sfeq",  OR1K_CC_EQ>;
defm SFNE  : SF<0x1, "l.sfne",  OR1K_CC_NE>;
defm SFGTU : SF<0x2, "l.sfgtu", OR1K_CC_GTU>;
defm SFGEU : SF<0x3, "l.sfgeu", OR1K_CC_GEU>;
defm SFLTU : SF<0x4, "l.sfltu", OR1K_CC_LTU>;
defm SFLEU : SF<0x5, "l.sfleu", OR1K_CC_LEU>;
defm SFGTS : SF<0xa, "l.sfgts", OR1K_CC_GT>;
defm SFGES : SF<0xb, "l.sfges", OR1K_CC_GE>;
defm SFLTS : SF<0xc, "l.sflts", OR1K_CC_LT>;
defm SFLES : SF<0xd, "l.sfles", OR1K_CC_LE>;
}
// -------------------------------------------------- //
// ALU instructions
// -------------------------------------------------- //

class ALU_RI<bits<4> subOp, dag outs, dag ins, string asmstr,
             list<dag> pattern>
  : InstRI<subOp, outs, ins, asmstr, pattern> {
  bits<5> rD;
  bits<5> rA;
  bits<16> imm16;

  let Inst{25-21} = rD;
  let Inst{20-16} = rA;
  let Inst{15-0} = imm16;
}

class ALU_RIs<bits<4> subOp, string asmstr, SDNode OpNode>
  : ALU_RI<subOp, (outs GPR:$rD), (ins GPR:$rA, s16imm:$imm16),
           !strconcat(asmstr, "\t$rD, $rA, $imm16"),
           [(set GPR:$rD, (OpNode GPR:$rA, immSExt16:$imm16))]>;

class ALU_RIz<bits<4> subOp, string asmstr, SDNode OpNode>
  : ALU_RI<subOp, (outs GPR:$rD), (ins GPR:$rA, i32imm:$imm16),
           !strconcat(asmstr, "\t$rD, $rA, $imm16"),
           [(set GPR:$rD, (OpNode GPR:$rA, immZExt16:$imm16))]>;

let isAsCheapAsAMove = 1 in {
  def ADDI  : ALU_RIs<0x7, "l.addi", add>;
  def ADDIC : ALU_RIs<0x8, "l.addic", add>;
  def ANDI  : ALU_RIz<0x9, "l.andi", and>;
  def ORI   : ALU_RIz<0xa, "l.ori", or>;
  def XORI  : ALU_RIs<0xb, "l.xori", xor>;
}

let Predicates=[HasMul] in
  def MULI  : ALU_RIs<0xc, "l.muli", mul>;

class ALU_RR<bits<4> subOp, string asmstr, list<dag> pattern>
  : InstRR<0x8, (outs GPR:$rD), (ins GPR:$rA, GPR:$rB),
           !strconcat(asmstr, "\t$rD, $rA, $rB"), pattern> {
  bits<5> rD;
  bits<5> rA;
  bits<5> rB;
  bits<2> op2;
  bits<4> op3;

  let Inst{25-21} = rD;
  let Inst{20-16} = rA;
  let Inst{15-11} = rB;

  let Inst{9-8} = op2;
  let Inst{3-0} = op3;

  let op2 = 0;
  let op3 = subOp;
}

class ALU1_RR<bits<4> subOp, string asmstr, SDNode OpNode>
  : ALU_RR<subOp, asmstr,
           [(set GPR:$rD, (OpNode (i32 GPR:$rA), (i32 GPR:$rB)))]>;

let isAsCheapAsAMove = 1 in {
  let isCommutable=1 in {
    def ADD  : ALU1_RR<0x0, "l.add", add>;
    def ADDC : ALU1_RR<0x1, "l.addc", add>;
  }
  def SUB  : ALU1_RR<0x2, "l.sub", sub>;
  let isCommutable=1 in {
    def AND  : ALU1_RR<0x3, "l.and", and>;
    def OR   : ALU1_RR<0x4, "l.or", or>;
    def XOR  : ALU1_RR<0x5, "l.xor", xor>;
 }
}

let Uses = [SR], Predicates = [HasCmov] in
  def CMOV : ALU1_RR<0xe, "l.cmov", OR1Kselectcc>;

let isCommutable=1, Predicates=[HasMul] in {
  def MUL  : ALU1_RR<0x6, "l.mul", mul>;
  def MULU : ALU1_RR<0xb, "l.mulu", mul>;
}
let Predicates=[HasDiv] in {
  def DIV  : ALU1_RR<0x9, "l.div", sdiv>;
  def DIVU : ALU1_RR<0xa, "l.divu", udiv>;
}

class ALU2_RR<bits<2> subOp1, bits<4> subOp2, string asmstr, SDNode OpNode>
  : InstRR<0x8, (outs GPR:$rD), (ins GPR:$rA),
           !strconcat(asmstr, "\t$rD, $rA"),
           [(set GPR:$rD, (OpNode GPR:$rA))]> {
  bits<5> rD;
  bits<5> rA;
  bits<2> op2;
  bits<4> op3;

  let Inst{25-21} = rD;
  let Inst{20-16} = rA;

  let Inst{9-8} = op2;
  let Inst{3-0} = op3;

  let op2 = subOp1;
  let op3 = subOp2;
}

def FF1 : ALU2_RR<0x0, 0xf, "l.ff1", OR1Kff1>;
def FL1 : ALU2_RR<0x1, 0xf, "l.fl1", OR1Kfl1>;

// -------------------------------------------------- //
// SHIFT instructions
// -------------------------------------------------- //

class SHIFT_RR<bits<2> op2Val, string asmstr, SDNode OpNode>
  : InstRR<0x8, (outs GPR:$rD), (ins GPR:$rA, GPR:$rB),
           !strconcat(asmstr, "\t$rD, $rA, $rB"),
           [(set GPR:$rD, (OpNode GPR:$rA, GPR:$rB))]> {
  bits<5> rD;
  bits<5> rA;
  bits<5> rB;
  bits<2> op2;
  bits<2> op3;
  bits<4> op4;

  let Inst{25-21} = rD;
  let Inst{20-16} = rA;
  let Inst{15-11} = rB;

  let Inst{9-8} = op2;
  let Inst{7-6} = op3;
  let Inst{3-0} = op4;

  let op2 = 0;
  let op3 = op2Val;
  let op4 = 0x8;
}

class SHIFT_RI<bits<2> op2Val, string asmstr, SDNode OpNode>
  : InstRI<0xE, (outs GPR:$rD), (ins GPR:$rA, i32imm:$imm),
           !strconcat(asmstr, "i\t$rD, $rA, $imm"),
           [(set GPR:$rD, (OpNode GPR:$rA, immZExt6:$imm))]> {
  bits<5> rD;
  bits<5> rA;
  bits<2> op2;
  bits<6> imm;

  let Inst{25-21} = rD;
  let Inst{20-16} = rA;
  let Inst{7-6} = op2;
  let Inst{5-0} = imm;

  let op2 = op2Val;
}

multiclass SHIFT<bits<2> op2Val, string asmstr, SDNode OpNode> {
  def _rr : SHIFT_RR<op2Val, asmstr, OpNode>;
  def _ri : SHIFT_RI<op2Val, asmstr, OpNode>;
}


defm SLL : SHIFT<0x0, "l.sll", shl>;
defm SRL : SHIFT<0x1, "l.srl", srl>;
defm SRA : SHIFT<0x2, "l.sra", sra>;
let Predicates=[HasRor] in
  defm ROR : SHIFT<0x3, "l.ror", rotr>;



// -------------------------------------------------- //
// STORE instructions
// -------------------------------------------------- //

class STORE<bits<4> subOp, string asmstring, list<dag> pattern>
  : InstRR<subOp, (outs), (ins GPR:$rB, MEMri:$dst),
          !strconcat(asmstring, "\t$dst, $rB"), pattern> {
  bits<21> dst;
  bits<5> rB;
  
  let Inst{25-21} = dst{15-11};
  let Inst{20-16} = dst{20-16};
  let Inst{15-11} = rB;
  let Inst{10-0} = dst{10-0};
}

class STOREi32<bits<4> subOp, string asmstring, PatFrag opNode>
  : STORE<subOp, asmstring, [(opNode (i32 GPR:$rB), ADDRri:$dst)]>;


//def SD : STORE<0x4, "l.sd">;
def SW : STOREi32<0x5, "l.sw", store>;
def SB : STOREi32<0x6, "l.sb", truncstorei8>;
def SH : STOREi32<0x7, "l.sh", truncstorei16>;

// -------------------------------------------------- //
// LOAD instructions
// -------------------------------------------------- //

class LOAD<bits<4> subop, string asmstring, list<dag> pattern>
  : InstRI<subop, (outs GPR:$rD), (ins MEMri:$src),
           !strconcat(asmstring, "\t$rD, $src"), pattern> {
  bits<5> rD;
  bits<21> src;

  let Inst{25-21} = rD;
  let Inst{20-0} = src;
}

class LOADi32<bits<4> subop, string asmstring, PatFrag opNode>
  : LOAD<subop, asmstring, [(set (i32 GPR:$rD), (opNode ADDRri:$src))]>;

//def LD : LOAD<0x0, "l.ld">;
def LWZ : LOADi32<0x1, "l.lwz", load>;
//def LWS : LOAD<0x2, "l.lws", load>;
def LBZ : LOADi32<0x3, "l.lbz", zextloadi8>;
def LBS : LOADi32<0x4, "l.lbs", sextloadi8>;
def LHZ : LOADi32<0x5, "l.lhz", zextloadi16>;
def LHS : LOADi32<0x6, "l.lhs", sextloadi16>;


// -------------------------------------------------- //
// BRANCH instructions
// -------------------------------------------------- //


class BRANCH<bits<4> subOp, string asmstring, list<dag> pattern>
  : InstBI<subOp, (outs), (ins brtarget:$dst),
           !strconcat(asmstring, "\t$dst"), pattern> {
  bits<26> dst;

  let Inst{25-0} = dst;

  let opcode = subOp;
}

class BRANCHL<bits<4> subOp, string asmstring>
  : InstBI<subOp, (outs), (ins calltarget:$dst),
           !strconcat(asmstring, "\t$dst"), []> {
  bits<26> dst;

  let Inst{25-0} = dst;

  let opcode = subOp;
}

class BRANCH_R<bits<4> subOp, string asmstring, list<dag> pattern>
  : InstBR<subOp, (outs), (ins GPR:$rB),
           !strconcat(asmstring, "\t$rB"), pattern> {
  bits<5> rB;

  let Inst{15-11} = rB;

  let opcode = subOp;
}

class BRANCHL_R<bits<4> subOp, string asmstring>
  : InstBR<subOp, (outs), (ins GPR:$rB),
           !strconcat(asmstring, "\t$rB"), [(call GPR:$rB)]> {
  bits<5> rB;

  let Inst{15-11} = rB;

  let opcode = subOp;
}

// Jump/Branch
let isBranch = 1, isTerminator = 1, hasDelaySlot=1 in {
  let isBarrier = 1 in {
    def J : BRANCH<0x0, "l.j", [(br bb:$dst)]>;
    let isIndirectBranch = 1 in {
      def JR : BRANCH_R<0x1, "l.jr", [(brind GPR:$rB)]>;
    }
  }
  let Uses = [SR] in {
    def BNF : BRANCH<0x3, "l.bnf", [(OR1Kbrcc bb:$dst, OR1K_COND_NEG)]>;
    def BF  : BRANCH<0x4, "l.bf",  [(OR1Kbrcc bb:$dst, OR1K_COND)]>;
  }
}

// Jump and link
let isCall=1, hasDelaySlot=1,
    Uses = [R1],
    // Potentially clobbered registers
    Defs = [R3, R4, R5, R6, R7, R8, R9, R11, R12, R13, R15, R17, R19, R21, R23,
            R25, R27, R29, R31] in {
  def JAL  : BRANCHL<0x1, "l.jal">;
  def JALR : BRANCHL_R<0x2, "l.jalr">;
}

/*

def MTSPR :

def MFSPR :


def NOP :


def SYS :
def TRAP :

def RFE :

*/

class NOP_I<bits<2> op2Val, string asmstr>
  : InstBI<0x5, (outs), (ins i16imm:$imm),
           !strconcat(asmstr, "\t$imm"), []> {
  bits<2> op2;
  bits<16> imm;

  let Inst{25-24} = op2;
  let Inst{15-0} = imm;

  let op2 = op2Val;
}

let neverHasSideEffects = 1 in
  def NOP : NOP_I<0x1, "l.nop">;

// FIXME: Do not use isCodeGenOnly = 1 to resolve conflicts
let isReturn = 1, isTerminator = 1, hasDelaySlot=1, isBarrier = 1, isCodeGenOnly = 1, Inst = 0x44004800 in {
  def RET : InstBR<0x1, (outs), (ins),
                   "l.jr\tr9",
                   [(retflag)]>;
}

// ADJCALLSTACKDOWN/UP implicitly use/def R1 because they may be expanded into
// a stack adjustment and the codegen must know that they may modify the stack
// pointer before prolog-epilog rewriting occurs.
// Pessimistically assume ADJCALLSTACKDOWN / ADJCALLSTACKUP will become
// sub / add which can clobber R1.
let Defs = [R1], Uses = [R1] in {
def ADJCALLSTACKDOWN : Pseudo<(outs), (ins i32imm:$amt),
                              "#ADJCALLSTACKDOWN $amt",
                              [(callseq_start timm:$amt)]>;
def ADJCALLSTACKUP   : Pseudo<(outs), (ins i32imm:$amt1, i32imm:$amt2),
                              "#ADJCALLSTACKUP $amt1 $amt2",
                              [(callseq_end timm:$amt1, timm:$amt2)]>;
}

let Defs = [R1], Uses = [R1] in {
  def ADJDYNALLOC : Pseudo<(outs GPR:$dst), (ins GPR:$src),
                    "#ADJDYNALLOC $dst $src",
                    [(set GPR:$dst, (OR1Kadjdynalloc GPR:$src))]>;
}

let Uses = [SR], usesCustomInserter = 1, Predicates = [NoCmov] in {
  def Select : Pseudo<(outs GPR:$dst), (ins GPR:$src, GPR:$src2),
                       "# Select PSEUDO $dst $src $src2",
                       [(set (i32 GPR:$dst),
                        (OR1Kselectcc (i32 GPR:$src), (i32 GPR:$src2)))]>;

  def Selectf32 : Pseudo<(outs GPR:$dst), (ins GPR:$src, GPR:$src2),
                         "# Select PSEUDO $dst $src $src2",
                         [(set (f32 GPR:$dst),
                          (OR1Kselectcc (f32 GPR:$src), (f32 GPR:$src2)))]>;
}


// PIC base construction.
let neverHasSideEffects = 1, isNotDuplicable = 1, Defs = [R9] in
  def GETPC : Pseudo<(outs), (ins i32imm:$label), 
  "#GETPC $label", []>;

class MOVHI_I <dag outs, dag ins, list<dag> pattern>
  : InstOR1K<outs, ins,  "l.movhi\t$rD, $imm", pattern> {
  bits<16> imm;
  bits<5> rD;
  let optype = 0;
  let opcode = 0x6;
  let Inst{16} = 0;
  let Inst{15-0} = imm;
  let Inst{25-21} = rD;
}

let isReMaterializable = 1, isAsCheapAsAMove = 1 in {
  def MOVHI : MOVHI_I<(outs GPR:$rD), (ins i32imm:$imm),
                      [(set GPR:$rD, i32hi16:$imm)]>;
}

//===----------------------------------------------------------------------===//
// Non-Instruction Patterns
//===----------------------------------------------------------------------===//

// signed 16-bit immediate
def : Pat<(i32 immSExt16:$imm), (ADDI (i32 R0), imm:$imm)>;

// unsigned 16-bit immediate
def : Pat<(i32 immZExt16:$imm), (ORI (i32 R0), imm:$imm)>;

// arbitrary immediate
def : Pat<(i32 imm:$imm), (ORI (MOVHI (HI16 imm:$imm)), (LO16 imm:$imm))>;

// Calls
def : Pat<(call tglobaladdr:$dst), (JAL tglobaladdr:$dst)>;
def : Pat<(call texternalsym:$dst), (JAL texternalsym:$dst)>;

// Loads
def : Pat<(extloadi8  ADDRri:$src), (i32 (LBZ ADDRri:$src))>;
def : Pat<(extloadi16 ADDRri:$src), (i32 (LHZ ADDRri:$src))>;

// GlobalAddress, ExternalSymbol, Jumptable, ConstantPool
def : Pat<(OR1KHi tglobaladdr:$dst), (MOVHI tglobaladdr:$dst)>;
def : Pat<(OR1KLo tglobaladdr:$dst), (ORI (i32 R0), tglobaladdr:$dst)>;
def : Pat<(OR1KHi texternalsym:$dst), (MOVHI texternalsym:$dst)>;
def : Pat<(OR1KLo texternalsym:$dst), (ORI (i32 R0), texternalsym:$dst)>;
def : Pat<(OR1KHi tblockaddress:$dst), (MOVHI tblockaddress:$dst)>;
def : Pat<(OR1KLo tblockaddress:$dst), (ORI (i32 R0), tblockaddress:$dst)>;
def : Pat<(OR1KHi tjumptable:$dst), (MOVHI tjumptable:$dst)>;
def : Pat<(OR1KLo tjumptable:$dst), (ORI (i32 R0), tjumptable:$dst)>;
def : Pat<(OR1KHi tconstpool:$dst), (MOVHI tconstpool:$dst)>;
def : Pat<(OR1KLo tconstpool:$dst), (ORI (i32 R0), tconstpool:$dst)>;

def : Pat<(or GPR:$hi, (OR1KLo tglobaladdr:$lo)),
          (ORI GPR:$hi, tglobaladdr:$lo)>;
def : Pat<(or GPR:$hi, (OR1KLo texternalsym:$lo)),
          (ORI GPR:$hi, texternalsym:$lo)>;
def : Pat<(or GPR:$hi, (OR1KLo tblockaddress:$lo)),
          (ORI GPR:$hi, tblockaddress:$lo)>;
def : Pat<(or GPR:$hi, (OR1KLo tjumptable:$lo)),
          (ORI GPR:$hi, tjumptable:$lo)>;
def : Pat<(or GPR:$hi, (OR1KLo tconstpool:$lo)),
          (ORI GPR:$hi, tconstpool:$lo)>;
//===----------------------------------------------------------------------===//
// Floating Point Support
//===----------------------------------------------------------------------===//

include "OR1KInstrFPU.td"
