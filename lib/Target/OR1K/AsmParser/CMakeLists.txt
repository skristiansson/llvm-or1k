add_lvm_library(LLVMOR1KASMParser
  OR1KAsmParser.cpp
  )

add_dependencies(LLVMOR1KAsmParser OR1KCommonTableGen)