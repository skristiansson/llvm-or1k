//===- OR1KRegisterInfo.td - OR1K Register defs ------------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Declarations that describe the OR1K register file
//===----------------------------------------------------------------------===//

class OR1KReg<string n> : Register<n> {
  field bits<5> Num;
  let Namespace = "OR1K";
}

let Namespace = "OR1K" in {
def sub_even : SubRegIndex;
def sub_odd  : SubRegIndex;
}

// Registers are identified with 5-bit ID numbers.
// Ri - 32-bit integer registers
class Ri<bits<5> num, string n> : OR1KReg<n> {
  let Num = num;
}

// Integer registers
def R0 : Ri< 0, "r0">, DwarfRegNum<[0]>;
def R1 : Ri< 1, "r1">, DwarfRegNum<[1]>;
def R2 : Ri< 2, "r2">, DwarfRegNum<[2]>;
def R3 : Ri< 3, "r3">, DwarfRegNum<[3]>;
def R4 : Ri< 4, "r4">, DwarfRegNum<[4]>;
def R5 : Ri< 5, "r5">, DwarfRegNum<[5]>;
def R6 : Ri< 6, "r6">, DwarfRegNum<[6]>;
def R7 : Ri< 7, "r7">, DwarfRegNum<[7]>;
def R8 : Ri< 8, "r8">, DwarfRegNum<[8]>;
def R9 : Ri< 9, "r9">, DwarfRegNum<[9]>;
def R10 : Ri<10, "r10">, DwarfRegNum<[10]>;
def R11 : Ri<11, "r11">, DwarfRegNum<[11]>;
def R12 : Ri<12, "r12">, DwarfRegNum<[12]>;
def R13 : Ri<13, "r13">, DwarfRegNum<[13]>;
def R14 : Ri<14, "r14">, DwarfRegNum<[14]>;
def R15 : Ri<15, "r15">, DwarfRegNum<[15]>;
def R16 : Ri<16, "r16">, DwarfRegNum<[16]>;
def R17 : Ri<17, "r17">, DwarfRegNum<[17]>;
def R18 : Ri<18, "r18">, DwarfRegNum<[18]>;
def R19 : Ri<19, "r19">, DwarfRegNum<[19]>;
def R20 : Ri<20, "r20">, DwarfRegNum<[20]>;
def R21 : Ri<21, "r21">, DwarfRegNum<[21]>;
def R22 : Ri<22, "r22">, DwarfRegNum<[22]>;
def R23 : Ri<23, "r23">, DwarfRegNum<[23]>;
def R24 : Ri<24, "r24">, DwarfRegNum<[24]>;
def R25 : Ri<25, "r25">, DwarfRegNum<[25]>;
def R26 : Ri<26, "r26">, DwarfRegNum<[26]>;
def R27 : Ri<27, "r27">, DwarfRegNum<[27]>;
def R28 : Ri<28, "r28">, DwarfRegNum<[28]>;
def R29 : Ri<29, "r29">, DwarfRegNum<[29]>;
def R30 : Ri<30, "r30">, DwarfRegNum<[30]>;
def R31 : Ri<31, "r31">, DwarfRegNum<[31]>;

// Register classes.
//
// FIXME: the register order should be defined in terms of the preferred
// allocation order...
//
def GPR : RegisterClass<"OR1K", [i32], 32, (add R3, R4, R5, R6, R7,
                                     R8,
                                     R12, R13, R14, R15,
                                     R16, R17, R18, R19, R20, R21, R22, R23,
                                     R24, R25, R26, R27, R28, R29, R30, R31,
                                     R10, // current thread info
                                     R11, // return value
                                     R1,  // stack ptr
                                     R2,  // frame ptr
                                     R9,  // link register
                                     R0   // constant 0
                                     )>;
