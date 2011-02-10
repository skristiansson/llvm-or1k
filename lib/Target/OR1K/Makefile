##===- lib/Target/OR1K/Makefile ---------------------------*- Makefile -*-===##
#
#                     The LLVM Compiler Infrastructure
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
#
##===----------------------------------------------------------------------===##

LEVEL = ../../..
LIBRARYNAME = LLVMOR1KCodeGen
TARGET = OR1K

# Make sure that tblgen is run, first thing.
BUILT_SOURCES = OR1KGenRegisterInfo.h.inc OR1KGenRegisterNames.inc \
                OR1KGenRegisterInfo.inc OR1KGenInstrNames.inc \
                OR1KGenInstrInfo.inc OR1KGenAsmWriter.inc \
                OR1KGenDAGISel.inc OR1KGenSubtarget.inc OR1KGenCallingConv.inc
#BUILT_SOURCES = SparcGenRegisterInfo.h.inc SparcGenRegisterNames.inc \
#                SparcGenRegisterInfo.inc SparcGenInstrNames.inc \
#                SparcGenInstrInfo.inc SparcGenAsmWriter.inc \
#                SparcGenDAGISel.inc SparcGenSubtarget.inc SparcGenCallingConv.inc

DIRS = AsmPrinter TargetInfo

include $(LEVEL)/Makefile.common

