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
BUILT_SOURCES = OR1KGenRegisterInfo.inc OR1KGenInstrInfo.inc \
		OR1KGenAsmWriter.inc OR1KGenAsmMatcher.inc OR1KGenDAGISel.inc \
		OR1KGenCodeEmitter.inc OR1KGenSubtargetInfo.inc OR1KGenCallingConv.inc \
		OR1KGenEDInfo.inc OR1KGenDisassemblerTables.inc

DIRS = AsmParser Disassembler InstPrinter TargetInfo MCTargetDesc

include $(LEVEL)/Makefile.common

