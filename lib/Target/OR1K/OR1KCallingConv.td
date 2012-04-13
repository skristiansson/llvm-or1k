//===- OR1KCallingConv.td - Calling Conventions OR1K -----*- tablegen -*-===//
// 
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
// 
//===----------------------------------------------------------------------===//
//
// This describes the calling conventions for the OR1K architectures.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Return Value Calling Conventions
//===----------------------------------------------------------------------===//

// OR1K 32-bit C return-value convention.
def RetCC_OR1K32 : CallingConv<[
  CCIfType<[i32], CCAssignToReg<[R11]>>,
  CCIfType<[f32], CCAssignToReg<[R11]>>
]>;

// OR1K 32-bit C Calling convention.
def CC_OR1K32 : CallingConv<[
  // Promote i8/i16 args to i32
  CCIfType<[i8, i16], CCPromoteToType<i32>>,

  // All arguments get passed in integer registers if there is space.
  CCIfType<[i32, f32], CCAssignToReg<[R3, R4, R5, R6, R7, R8]>>,
  
  // Alternatively, they are assigned to the stack in 4-byte aligned units.
  CCAssignToStack<4, 4>
]>;
