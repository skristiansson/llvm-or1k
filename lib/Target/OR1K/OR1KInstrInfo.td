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

def retflag       : SDNode<"OR1KISD::RET_FLAG", SDTNone,
                           [SDNPHasChain, SDNPOptInGlue]>;
def callseq_start : SDNode<"ISD::CALLSEQ_START", SDT_OR1KCallSeqStart,
                           [SDNPHasChain, SDNPOutGlue]>;
def callseq_end   : SDNode<"ISD::CALLSEQ_END",   SDT_OR1KCallSeqEnd,
                           [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;

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
  let MIOperandInfo = (ops GPR, i32imm);
}

// -------------------------------------------------- //
// Condition/SF instructions
// -------------------------------------------------- //

class SF_RR<bits<5> op2Val, string asmstr>
	: InstRR<0x9, (outs), (ins GPR:$rA, GPR:$rB),
		!strconcat(asmstr, "\t$rA, $rB"), []>
{
  bits<5> op2;
  bits<5> rA;
  bits<5> rB;

  let Inst{25-21} = op2;
  let Inst{20-16} = rA;
  let Inst{15-11} = rB;

  let op2 = op2Val;
}

class SF_RI<bits<5> op2Val, string asmstr>
	: InstRI<0xf, (outs), (ins GPR:$rA, i16imm:$imm),
		!strconcat(asmstr, "i\t$rA, $imm"), []>
{
  bits<5> op2;
  bits<5> rA;
  bits<16> imm16;

  let Inst{25-21} = op2;
  let Inst{20-16} = rA;
  let Inst{15-0} = imm16;

  let op2 = op2Val;
}


multiclass SF<bits<5> op2Val, string asmstr>
{
  def _rr : SF_RR<op2Val, asmstr>;
  def _ri : SF_RI<op2Val, asmstr>;
}

// l.sfxx instructions

defm SFEQ : SF<0x0, "l.sfeq">;
defm SFNE : SF<0x1, "l.sfne">;
defm SFGTU : SF<0x2, "l.sfgtu">;
defm SFGEU : SF<0x3, "l.sfgeu">;
defm SFLTU : SF<0x4, "l.sfltu">;
defm SFLEU : SF<0x5, "l.sfleu">;
defm SFGTS : SF<0xa, "l.sfgts">;
defm SFGES : SF<0xb, "l.sfges">;
defm SFLTS : SF<0xc, "l.sflts">;
defm SFLES : SF<0xd, "l.sfles">;

// -------------------------------------------------- //
// ALU instructions
// -------------------------------------------------- //

class ALU_RIs<bits<4> subOp, string asmstr, SDNode OpNode>
	: InstRI<subOp, (outs GPR:$rD), (ins GPR:$rA, s16imm:$imm16),
		!strconcat(asmstr, "\t$rD, $rA, $imm16"),
		[(set GPR:$rD, (OpNode GPR:$rA, immSExt16:$imm16))]>
{
  bits<5> rD;
  bits<5> rA;
  bits<16> imm16;

  let Inst{25-21} = rD;
  let Inst{20-16} = rA;
  let Inst{15-0} = imm16;
}

class ALU_RIz<bits<4> subOp, string asmstr, SDNode OpNode>
        : InstRI<subOp, (outs GPR:$rD), (ins GPR:$rA, i32imm:$imm16),
                !strconcat(asmstr, "\t$rD, $rA, $imm16"),
                [(set GPR:$rD, (OpNode GPR:$rA, immZExt16:$imm16))]>
{
  bits<5> rD;
  bits<5> rA;
  bits<16> imm16;

  let Inst{25-21} = rD;
  let Inst{20-16} = rA;
  let Inst{15-0} = imm16;
}

let isAsCheapAsAMove = 1 in {
def ADDI  : ALU_RIs<0x7, "l.addi", add>;
def ADDIC : ALU_RIs<0x8, "l.addic", add>;
def ANDI  : ALU_RIz<0x9, "l.andi", and>;
def ORI   : ALU_RIz<0xa, "l.ori", or>;
def XORI  : ALU_RIs<0xb, "l.xori", xor>;
}
def MULI  : ALU_RIs<0xc, "l.muli", mul>;



class ALU_RR<bits<4> subOp, string asmstr, SDNode OpNode>
	: InstRR<0x8, (outs GPR:$rD), (ins GPR:$rA, GPR:$rB),
		!strconcat(asmstr, "\t$rD, $rA, $rB"),
		[(set GPR:$rD, (OpNode GPR:$rA, GPR:$rB))]>
{
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

let isCommutable=1, isAsCheapAsAMove = 1 in {
def ADD  : ALU_RR<0x0, "l.add", add>;
def ADDC : ALU_RR<0x1, "l.addc", add>;
}
let isAsCheapAsAMove = 1 in {
def SUB  : ALU_RR<0x2, "l.sub", sub>;
}
let isCommutable=1, isAsCheapAsAMove = 1 in {
def AND  : ALU_RR<0x3, "l.and", and>;
def OR   : ALU_RR<0x4, "l.or", or>;
def XOR  : ALU_RR<0x5, "l.xor", xor>;
def MUL  : ALU_RR<0x6, "l.mul", mul>;
}



// -------------------------------------------------- //
// SHIFT instructions
// -------------------------------------------------- //

class SHIFT_RR<bits<2> op2Val, string asmstr, SDNode OpNode>
	: InstRR<0x8, (outs GPR:$rD), (ins GPR:$rA, GPR:$rB),
		!strconcat(asmstr, "\t$rD, $rA, $rB"),
		[(set GPR:$rD, (OpNode GPR:$rA, GPR:$rB))]>
{
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
		[(set GPR:$rD, (OpNode GPR:$rA, immZExt6:$imm))]>
{
  bits<5> rD;
  bits<5> rA;
  bits<2> op2;
  bits<6> imm6;

  let Inst{25-21} = rD;
  let Inst{20-16} = rA;
  let Inst{7-6} = op2;
  let Inst{5-0} = imm6;

  let op2 = op2Val;
}

multiclass SHIFT<bits<2> op2Val, string asmstr, SDNode OpNode>
{
  def _rr : SHIFT_RR<op2Val, asmstr, OpNode>;
  def _ri : SHIFT_RI<op2Val, asmstr, OpNode>;
}


defm SLL : SHIFT<0x0, "l.sll", shl>;
defm SRL : SHIFT<0x1, "l.srl", srl>;
defm SRA : SHIFT<0x2, "l.sra", sra>;
defm ROR : SHIFT<0x3, "l.ror", rotr>;



// -------------------------------------------------- //
// STORE instructions
// -------------------------------------------------- //

class STORE<bits<4> subOp, string asmstring, PatFrag opNode>
	: InstRR<subOp, (outs), (ins GPR:$rB, MEMri:$dst),
		!strconcat(asmstring, "\t$dst, $rB"),
		[(opNode (i32 GPR:$rB), ADDRri:$dst)]>
{
  bits<5> rA;
  bits<5> rB;
  bits<16> imm16;

  let Inst{25-21} = imm16{15-11};
  let Inst{20-16} = rA;
  let Inst{15-11} = rB;
  let Inst{10-0} = imm16{10-0};
}


//def SD : STORE<0x4, "l.sd">;
def SW : STORE<0x5, "l.sw", store>;
def SB : STORE<0x6, "l.sb", truncstorei8>;
def SH : STORE<0x7, "l.sh", truncstorei16>;

// -------------------------------------------------- //
// LOAD instructions
// -------------------------------------------------- //

class LOAD<bits<4> subop, string asmstring, PatFrag opNode>
	: InstRI<subop, (outs GPR:$rD), (ins MEMri:$src),
		!strconcat(asmstring, "\t$rD, $src"),
		[(set (i32 GPR:$rD), (opNode ADDRri:$src))]>
{
  bits<5> rD;
  bits<5> rA;
  bits<16> imm16;

  let Inst{25-21} = rD;
  let Inst{20-16} = rA;
  let Inst{15-0} = imm16;
}


//def LD : LOAD<0x0, "l.ld">;
def LWZ : LOAD<0x1, "l.lwz", load>;
//def LWS : LOAD<0x2, "l.lws", load>;
def LBZ : LOAD<0x3, "l.lbz", zextloadi8>;
def LBS : LOAD<0x4, "l.lbs", sextloadi8>;
def LHZ : LOAD<0x5, "l.lhz", zextloadi16>;
def LHS : LOAD<0x6, "l.lhs", sextloadi16>;


// -------------------------------------------------- //
// BRANCH instructions
// -------------------------------------------------- //


class BRANCH<bits<4> subOp, string asmstring>
        : InstBR<subOp, (outs), (ins i32imm:$imm),
		!strconcat(asmstring, "\t$imm"),
		[]>
{
  bits<26> imm26;

  let Inst{25-0} = imm26;

  let opcode = subOp;
}

def J   : BRANCH<0x0, "l.j">;
def JAL : BRANCH<0x1, "l.jal">;
def BNF : BRANCH<0x2, "l.bnf">;
def BF  : BRANCH<0x3, "l.bf">;

class BRANCH_R<bits<4> subOp, string asmstring>
        : InstBR<subOp, (outs), (ins GPR:$rB),
		!strconcat(asmstring, "\t$rB"),
		[]>
{
  bits<5> rB;

  let Inst{15-11} = rB;

  let opcode = subOp;
}

def JR   : BRANCH_R<0x1, "l.jr">;
def JALR : BRANCH_R<0x2, "l.jalr">;

/*

def MTSPR :

def MFSPR :


def NOP :


def SYS :
def TRAP :

def RFE :

*/

class NOP_I<bits<2> op2Val, string asmstr>
	: InstRI<0x5, (outs), (ins i16imm:$imm),
		!strconcat(asmstr, "\t$imm"), []>
{
  bits<2> op2;
  bits<16> imm16;

  let Inst{25-24} = op2;
  let Inst{15-0} = imm16;

  let op2 = op2Val;
}

let neverHasSideEffects = 1 in
  def NOP : NOP_I<0x1, "l.nop">;

let isReturn = 1, isTerminator = 1, hasDelaySlot=1, isBarrier = 1 in {
  def RET  : InstBR<0x1, (outs), (ins),
                    "l.jr\tr9",
                    [(retflag)]>;
}

// ADJCALLSTACKDOWN/UP implicitly use/def R1 because they may be expanded into
// a stack adjustment and the codegen must know that they may modify the stack
// pointer before prolog-epilog rewriting occurs.
// Pessimistically assume ADJCALLSTACKDOWN / ADJCALLSTACKUP will become
// sub / add which can clobber R1.
let Defs = [R1], Uses = [R1] in {
def ADJCALLSTACKDOWN : InstOR1K<(outs), (ins i32imm:$amt),
                                    "#ADJCALLSTACKDOWN",
                                    [(callseq_start timm:$amt)]>;
def ADJCALLSTACKUP   : InstOR1K<(outs), (ins i32imm:$amt1, i32imm:$amt2),
                                    "#ADJCALLSTACKUP",
                                    [(callseq_end timm:$amt1, timm:$amt2)]>;
}

class MOVHI_I
        : InstOR1K<(outs GPR:$rD), (ins i32imm:$imm),
                   "l.movhi\t{$rD, $imm}",
                   [(set GPR:$rD, i32hi16:$imm)]>
{
  bits<16> imm16;
  let optype = 0;
  let opcode = 0x6;
  let Inst{16} = 0;
  let Inst{15-0} = imm16;
}

let isReMaterializable = 1, isAsCheapAsAMove = 1 in {
def MOVHI : MOVHI_I;
}

//===----------------------------------------------------------------------===//
// Non-Instruction Patterns
//===----------------------------------------------------------------------===//

// signed 16-bit immediate
def : Pat<(i32 immSExt16:$imm), (ADDI R0, imm:$imm)>;

// unsigned 16-bit immediate
def : Pat<(i32 immZExt16:$imm), (ORI R0, imm:$imm)>;

// arbitrary immediate
def : Pat<(i32 imm:$imm), (ORI (MOVHI (HI16 imm:$imm)), (LO16 imm:$imm))>;

