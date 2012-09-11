//===-- OR1KInstrFPU.td - OR1K FPU Instruction Information -*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file describes the OR1K FPU instruction set.
//
//===----------------------------------------------------------------------===//

def fimm   : Operand<f32>;

def HI16f32 : SDNodeXForm<fpimm, [{
  uint64_t Val = *N->getValueAPF().bitcastToAPInt().getRawData();
  return CurDAG->getTargetConstant(Val >> 16, MVT::i32);
}]>;

def LO16f32 : SDNodeXForm<fpimm, [{
  uint64_t Val = *N->getValueAPF().bitcastToAPInt().getRawData();
  return CurDAG->getTargetConstant(Val & 0xFFFFULL, MVT::i32);
}]>;

def f32lo16 : PatLeaf<(f32 fpimm), [{
  uint64_t Val = *N->getValueAPF().bitcastToAPInt().getRawData();
  return (Val & 0xFFFFULL) == Val;
}], LO16f32>;

def f32hi16 : PatLeaf<(f32 fpimm), [{
  uint64_t Val = *N->getValueAPF().bitcastToAPInt().getRawData();
  return (Val & 0xFFFF0000ULL) == Val;
}], HI16f32>;

//===----------------------------------------------------------------------===//
// ALU instructions
//===----------------------------------------------------------------------===//

class AluF<bits<8> subOp, string asmstr, SDNode OpNode>
  : InstFRR<subOp, (outs GPR:$rD), (ins GPR:$rA, GPR:$rB),
           !strconcat(asmstr, "\t$rD, $rA, $rB"),
           [(set (f32 GPR:$rD), (OpNode (f32 GPR:$rA), (f32 GPR:$rB)))]> {
  bits<5> rD;
  bits<5> rA;
  bits<5> rB;

  let Inst{25-21} = rD;
  let Inst{20-16} = rA;
  let Inst{15-11} = rB;
}

def ADDrrf32 : AluF<0x00, "lf.add.s", fadd>;
def SUBrrf32 : AluF<0x01, "lf.sub.s", fsub>;
def MULrrf32 : AluF<0x02, "lf.mul.s", fmul>;
def DIVrrf32 : AluF<0x03, "lf.div.s", fdiv>;
def REMrrf32 : AluF<0x06, "lf.rem.s", frem>;

//===----------------------------------------------------------------------===//
// fp->int and int->fp conversion
//===----------------------------------------------------------------------===//
class ConvF<bits<8> subOp, string asmstr, list<dag> pattern>
  : InstFRR<subOp, (outs GPR:$rD), (ins GPR:$rA),
           !strconcat(asmstr, "\t$rD, $rA"), pattern> {
  bits<5> rD;
  bits<5> rA;

  let Inst{25-21} = rD;
  let Inst{20-16} = rA;
  let Inst{15-11} = 0;
}

def ITOFf32 : ConvF<0x04, "lf.itof.s",
                    [(set (f32 GPR:$rD), (sint_to_fp (i32 GPR:$rA)))]>;
def FTOIf32 : ConvF<0x05, "lf.ftoi.s",
                    [(set (i32 GPR:$rD), (fp_to_sint (f32 GPR:$rA)))]>;

//===----------------------------------------------------------------------===//
// Condition/SF instructions
//===----------------------------------------------------------------------===//

class SetFlagFRR<bits<8> subOp, string asmstr, PatLeaf Cond>
  : InstFRR<subOp, (outs), (ins GPR:$rA, GPR:$rB),
           !strconcat(asmstr, "\t$rA, $rB"),
           [(OR1Ksetflag (f32 GPR:$rA), (f32 GPR:$rB), Cond)]> {
  bits<5> rA;
  bits<5> rB;

  let Inst{20-16} = rA;
  let Inst{15-11} = rB;
}

// FIXME: Do not use isCodeGenOnly = 1 to resolve conflicts
let Defs = [SR], hasSideEffects = 1 in {
  def SFEQrrf32 : SetFlagFRR<0x08, "lf.sfeq.s", OR1K_CC_EQ>;
  def SFNErrf32 : SetFlagFRR<0x09, "lf.sfne.s", OR1K_CC_NE>;
  def SFGTrrf32 : SetFlagFRR<0x0a, "lf.sfgt.s", OR1K_CC_GT>;
  def SFGErrf32 : SetFlagFRR<0x0b, "lf.sfge.s", OR1K_CC_GE>;
  def SFLTrrf32 : SetFlagFRR<0x0c, "lf.sflt.s", OR1K_CC_LT>;
  def SFLErrf32 : SetFlagFRR<0x0d, "lf.sfle.s", OR1K_CC_LE>;
  let isCodeGenOnly = 1 in {
    def SFEQOrrf32 : SetFlagFRR<0x08, "lf.sfeq.s", OR1K_CC_EQO>;
    def SFNEOrrf32 : SetFlagFRR<0x09, "lf.sfne.s", OR1K_CC_NEO>;
    def SFGTOrrf32 : SetFlagFRR<0x0a, "lf.sfgt.s", OR1K_CC_GTO>;
    def SFGEOrrf32 : SetFlagFRR<0x0b, "lf.sfge.s", OR1K_CC_GEO>;
    def SFLTOrrf32 : SetFlagFRR<0x0c, "lf.sflt.s", OR1K_CC_LTO>;
    def SFLEOrrf32 : SetFlagFRR<0x0d, "lf.sfle.s", OR1K_CC_LEO>;
  }
}

// FIXME: Do not use isCodeGenOnly = 1 to resolve conflicts
let Uses = [SR], Predicates = [HasCmov], isCodeGenOnly = 1 in
  def CMOVf32 : ALU_RR<0xe, "l.cmov",
           [(set GPR:$rD, (OR1Kselectcc (f32 GPR:$rA),(f32 GPR:$rB)))]>;

//===----------------------------------------------------------------------===//
// LOAD/STORE instructions
//===----------------------------------------------------------------------===//

class LOADf32<bits<4> subop, string asmstring, PatFrag opNode>
  : LOAD<subop, asmstring, [(set (f32 GPR:$rD), (opNode ADDRri:$src))]>;

class STOREf32<bits<4> subOp, string asmstring, PatFrag opNode>
  : STORE<subOp, asmstring, [(opNode (f32 GPR:$rB), ADDRri:$dst)]>;

// FIXME: Do not use isCodeGenOnly = 1 to resolve conflicts
let isCodeGenOnly = 1 in {
def LWZf32 : LOADf32<0x1, "l.lwz", load>;
def SWf32 : STOREf32<0x5, "l.sw", store>;
}

//===----------------------------------------------------------------------===//
// Non-fpu instructions with fp immediates
//===----------------------------------------------------------------------===//

// FIXME: Do not use isCodeGenOnly = 1 to resolve conflicts
let isAsCheapAsAMove = 1, isCodeGenOnly = 1 in {
  def MOVHIf32 : MOVHI_I<(outs GPR:$rD), (ins fimm:$imm),
                         [(set GPR:$rD, f32hi16:$imm)]>;
  def ORIf32 : ALU_RI<0xa, (outs GPR:$rD), (ins GPR:$rA, fimm:$imm),
                      "l.ori\t$rD, $rA, $imm", []>;
}

//===----------------------------------------------------------------------===//
// Non-Instruction Patterns
//===----------------------------------------------------------------------===//

def : Pat<(f32 fpimm:$imm), (ORIf32 (MOVHIf32 (HI16f32 fpimm:$imm)),
                            (LO16f32 fpimm:$imm))>;
def : Pat<(f32 f32lo16:$imm), (ORIf32 (i32 R0), fpimm:$imm)>;
