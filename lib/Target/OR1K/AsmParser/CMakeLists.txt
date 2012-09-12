add_llvm_library(LLVMOR1KAsmParser
  OR1KAsmParser.cpp
  )

add_dependencies(LLVMOR1KAsmParser OR1KCommonTableGen)