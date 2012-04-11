//===- OR1KRegisterInfo.td - OR1K Register defs ----------*- tablegen -*-===//
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

class OR1KCtrlReg<string n>: Register<n> {
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

/*
// Rf - 32-bit floating-point registers
class Rf<bits<5> num, string n> : SparcReg<n> {
  let Num = num;
}
// Rd - Slots in the FP register file for 64-bit floating-point values.
class Rd<bits<5> num, string n, list<Register> subregs> : SparcReg<n> {
  let Num = num;
  let SubRegs = subregs;
  let SubRegIndices = [sub_even, sub_odd];
}

// Control Registers
def ICC : SparcCtrlReg<"ICC">;
def FCC : SparcCtrlReg<"FCC">;
*/

// Integer registers
def R0 : Ri< 0, "R0">, DwarfRegNum<[0]>;
def R1 : Ri< 1, "R1">, DwarfRegNum<[1]>;
def R2 : Ri< 2, "R2">, DwarfRegNum<[2]>; 
def R3 : Ri< 3, "R3">, DwarfRegNum<[3]>;
def R4 : Ri< 4, "R4">, DwarfRegNum<[4]>;
def R5 : Ri< 5, "R5">, DwarfRegNum<[5]>; 
def R6 : Ri< 6, "R6">, DwarfRegNum<[6]>;
def R7 : Ri< 7, "R7">, DwarfRegNum<[7]>;
def R8 : Ri< 8, "R8">, DwarfRegNum<[8]>;
def R9 : Ri< 9, "R9">, DwarfRegNum<[9]>;
def R10 : Ri<10, "R10">, DwarfRegNum<[10]>; 
def R11 : Ri<11, "R11">, DwarfRegNum<[11]>;
def R12 : Ri<12, "R12">, DwarfRegNum<[12]>;
def R13 : Ri<13, "R13">, DwarfRegNum<[13]>; 
def R14 : Ri<14, "R14">, DwarfRegNum<[14]>;
def R15 : Ri<15, "R15">, DwarfRegNum<[15]>;
def R16 : Ri<16, "R16">, DwarfRegNum<[16]>;
def R17 : Ri<17, "R17">, DwarfRegNum<[17]>;
def R18 : Ri<18, "R18">, DwarfRegNum<[18]>; 
def R19 : Ri<19, "R19">, DwarfRegNum<[19]>;
def R20 : Ri<20, "R20">, DwarfRegNum<[20]>;
def R21 : Ri<21, "R21">, DwarfRegNum<[21]>; 
def R22 : Ri<22, "R22">, DwarfRegNum<[22]>;
def R23 : Ri<23, "R23">, DwarfRegNum<[23]>;
def R24 : Ri<24, "R24">, DwarfRegNum<[24]>;
def R25 : Ri<25, "R25">, DwarfRegNum<[25]>;
def R26 : Ri<26, "R26">, DwarfRegNum<[26]>; 
def R27 : Ri<27, "R27">, DwarfRegNum<[27]>;
def R28 : Ri<28, "R28">, DwarfRegNum<[28]>;
def R29 : Ri<29, "R29">, DwarfRegNum<[29]>; 
def R30 : Ri<30, "R30">, DwarfRegNum<[30]>;
def R31 : Ri<31, "R31">, DwarfRegNum<[31]>;


/*
// Floating-point registers
def F0  : Rf< 0,  "F0">, DwarfRegNum<[32]>;
def F1  : Rf< 1,  "F1">, DwarfRegNum<[33]>;
def F2  : Rf< 2,  "F2">, DwarfRegNum<[34]>; 
def F3  : Rf< 3,  "F3">, DwarfRegNum<[35]>;
def F4  : Rf< 4,  "F4">, DwarfRegNum<[36]>;
def F5  : Rf< 5,  "F5">, DwarfRegNum<[37]>; 
def F6  : Rf< 6,  "F6">, DwarfRegNum<[38]>;
def F7  : Rf< 7,  "F7">, DwarfRegNum<[39]>;
def F8  : Rf< 8,  "F8">, DwarfRegNum<[40]>; 
def F9  : Rf< 9,  "F9">, DwarfRegNum<[41]>;
def F10 : Rf<10, "F10">, DwarfRegNum<[42]>;
def F11 : Rf<11, "F11">, DwarfRegNum<[43]>; 
def F12 : Rf<12, "F12">, DwarfRegNum<[44]>;
def F13 : Rf<13, "F13">, DwarfRegNum<[45]>;
def F14 : Rf<14, "F14">, DwarfRegNum<[46]>; 
def F15 : Rf<15, "F15">, DwarfRegNum<[47]>;
def F16 : Rf<16, "F16">, DwarfRegNum<[48]>;
def F17 : Rf<17, "F17">, DwarfRegNum<[49]>; 
def F18 : Rf<18, "F18">, DwarfRegNum<[50]>;
def F19 : Rf<19, "F19">, DwarfRegNum<[51]>;
def F20 : Rf<20, "F20">, DwarfRegNum<[52]>; 
def F21 : Rf<21, "F21">, DwarfRegNum<[53]>;
def F22 : Rf<22, "F22">, DwarfRegNum<[54]>;
def F23 : Rf<23, "F23">, DwarfRegNum<[55]>;
def F24 : Rf<24, "F24">, DwarfRegNum<[56]>;
def F25 : Rf<25, "F25">, DwarfRegNum<[57]>;
def F26 : Rf<26, "F26">, DwarfRegNum<[58]>; 
def F27 : Rf<27, "F27">, DwarfRegNum<[59]>;
def F28 : Rf<28, "F28">, DwarfRegNum<[60]>;
def F29 : Rf<29, "F29">, DwarfRegNum<[61]>; 
def F30 : Rf<30, "F30">, DwarfRegNum<[62]>;
def F31 : Rf<31, "F31">, DwarfRegNum<[63]>;

// Aliases of the F* registers used to hold 64-bit fp values (doubles)
def D0  : Rd< 0,  "F0", [F0,   F1]>, DwarfRegNum<[32]>;
def D1  : Rd< 2,  "F2", [F2,   F3]>, DwarfRegNum<[34]>; 
def D2  : Rd< 4,  "F4", [F4,   F5]>, DwarfRegNum<[36]>;
def D3  : Rd< 6,  "F6", [F6,   F7]>, DwarfRegNum<[38]>; 
def D4  : Rd< 8,  "F8", [F8,   F9]>, DwarfRegNum<[40]>;
def D5  : Rd<10, "F10", [F10, F11]>, DwarfRegNum<[42]>;
def D6  : Rd<12, "F12", [F12, F13]>, DwarfRegNum<[44]>;
def D7  : Rd<14, "F14", [F14, F15]>, DwarfRegNum<[46]>; 
def D8  : Rd<16, "F16", [F16, F17]>, DwarfRegNum<[48]>;
def D9  : Rd<18, "F18", [F18, F19]>, DwarfRegNum<[50]>; 
def D10 : Rd<20, "F20", [F20, F21]>, DwarfRegNum<[52]>;
def D11 : Rd<22, "F22", [F22, F23]>, DwarfRegNum<[54]>;
def D12 : Rd<24, "F24", [F24, F25]>, DwarfRegNum<[56]>;
def D13 : Rd<26, "F26", [F26, F27]>, DwarfRegNum<[58]>; 
def D14 : Rd<28, "F28", [F28, F29]>, DwarfRegNum<[60]>;
def D15 : Rd<30, "F30", [F30, F31]>, DwarfRegNum<[62]>;
*/

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
                                     R0  // constant 0
                                     )>;

/*
def FPRegs : RegisterClass<"SP", [f32], 32, [F0, F1, F2, F3, F4, F5, F6, F7, F8,
  F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22,
  F23, F24, F25, F26, F27, F28, F29, F30, F31]>;

def DFPRegs : RegisterClass<"SP", [f64], 64, [D0, D1, D2, D3, D4, D5, D6, D7,
  D8, D9, D10, D11, D12, D13, D14, D15]>;
*/
