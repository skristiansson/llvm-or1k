add_llvm_library(LLVMOR1KDesc
  OR1KAsmBackend.cpp
  OR1KELFObjectWriter.cpp
  OR1KMCAsmInfo.cpp
  OR1KMCCodeEmitter.cpp
  OR1KMCTargetDesc.cpp
  )

add_dependencies(LLVMOR1KDesc OR1KCommonTableGen)
