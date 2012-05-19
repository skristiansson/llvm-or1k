//===- OR1K.td - Describe the OR1K Target Machine --------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Target-independent interfaces which we are implementing
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// OR1K Subtarget features.
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Register File, Calling Conv, Instruction Descriptions
//===----------------------------------------------------------------------===//

include "OR1KRegisterInfo.td"
include "OR1KCallingConv.td"
include "OR1KInstrInfo.td"

def OR1KInstrInfo : InstrInfo;

//===----------------------------------------------------------------------===//
// OR1K subtarget features
//===----------------------------------------------------------------------===//
def FeatureMul   : SubtargetFeature<"mul", "HasMul", "true",
                                   "Enable hardware multiplier">;
def FeatureDiv   : SubtargetFeature<"div", "HasDiv", "true",
                                   "Enable hardware divider">;
def FeatureRor   : SubtargetFeature<"ror", "HasRor", "true",
                                   "Enable hardware rotate right">;
def FeatureCmov  : SubtargetFeature<"cmov", "HasCmov", "true",
                                    "Enable conditional move instruction">;

//===----------------------------------------------------------------------===//
// OR1K processors supported.
//===----------------------------------------------------------------------===//

class Proc<string Name, list<SubtargetFeature> Features>
 : Processor<Name, NoItineraries, Features>;

def : Proc<"generic", [FeatureMul, FeatureDiv]>;
def : Proc<"or1200",  [FeatureMul, FeatureDiv]>;

def OpenRISCInstPrinter : AsmWriter {
  string AsmWriterClassName  = "InstPrinter";
  bit isMCAsmWriter = 1;
}
//===----------------------------------------------------------------------===//
// Declare the target which we are implementing
//===----------------------------------------------------------------------===//

def OR1K : Target {
  // Pull in Instruction Info:
  let InstructionSet = OR1KInstrInfo;
  let AssemblyWriters = [OpenRISCInstPrinter];
}
