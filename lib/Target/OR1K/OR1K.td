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
//
 
/*
def FeatureV9
  : SubtargetFeature<"v9", "IsV9", "true",
                     "Enable SPARC-V9 instructions">;
def FeatureV8Deprecated
  : SubtargetFeature<"deprecated-v8", "V8DeprecatedInsts", "true",
                     "Enable deprecated V8 instructions in V9 mode">;
def FeatureVIS
  : SubtargetFeature<"vis", "IsVIS", "true",
                     "Enable UltraSPARC Visual Instruction Set extensions">;
*/

//===----------------------------------------------------------------------===//
// Register File, Calling Conv, Instruction Descriptions
//===----------------------------------------------------------------------===//

include "OR1KRegisterInfo.td"
include "OR1KCallingConv.td"
include "OR1KInstrInfo.td"

def OR1KInstrInfo : InstrInfo;

//===----------------------------------------------------------------------===//
// OR1K processors supported.
//===----------------------------------------------------------------------===//

class Proc<string Name, list<SubtargetFeature> Features>
 : Processor<Name, NoItineraries, Features>;

def : Proc<"generic", []>;
def : Proc<"or1200",  []>;

//===----------------------------------------------------------------------===//
// Declare the target which we are implementing
//===----------------------------------------------------------------------===//

def OR1K : Target {
  // Pull in Instruction Info:
  let InstructionSet = OR1KInstrInfo;
}
