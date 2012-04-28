include_directories( ${CMAKE_CURRENT_BINARY_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_llvm_library(LLVMOR1KAsmPrinter
  OR1KInstPrinter.cpp
  )

add_dependencies(LLVMOR1KAsmPrinter OR1KCommonTableGen)
