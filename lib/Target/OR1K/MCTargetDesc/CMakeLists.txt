add_llvm_library(LLVMOR1KDesc
  OR1KMCTargetDesc.cpp
  OR1KMCAsmInfo.cpp
  OR1KMCCodeEmitter.cpp
  )

add_dependencies(LLVMOR1KDesc OR1KCommonTableGen)
