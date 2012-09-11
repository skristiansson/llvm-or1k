set(LLVM_TARGET_DEFINITIONS OR1K.td)

tablegen(LLVM OR1KGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM OR1KGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM OR1KGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM OR1KGenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM OR1KGenCodeEmitter.inc -gen-emitter)
tablegen(LLVM OR1KGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM OR1KGenCallingConv.inc -gen-callingconv)
tablegen(LLVM OR1KGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM OR1KGenDisassemblerTables.inc -gen-disassembler)
tablegen(LLVM OR1KGenEDInfo.inc -gen-enhanced-disassembly-info)
add_public_tablegen_target(OR1KCommonTableGen)

add_llvm_target(OR1KCodeGen
  OR1KDelaySlotFiller.cpp
  OR1KISelDAGToDAG.cpp
  OR1KISelLowering.cpp
  OR1KInstrInfo.cpp
  OR1KFrameLowering.cpp
  OR1KMachineFunctionInfo.cpp
  OR1KRegisterInfo.cpp
  OR1KSubtarget.cpp
  OR1KTargetMachine.cpp
  OR1KSelectionDAGInfo.cpp
  OR1KAsmPrinter.cpp
  OR1KMCInstLower.cpp
  OR1KELFWriterInfo.cpp
  )

add_subdirectory(InstPrinter)
add_subdirectory(Disassembler)
add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)
add_subdirectory(AsmParser)