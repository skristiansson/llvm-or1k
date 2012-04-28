include_directories( ${CMAKE_CURRENT_BINARY_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_llvm_library(LLVMOR1KInfo
  OR1KTargetInfo.cpp
  )

add_dependencies(LLVMOR1KInfo OR1KCommonTableGen)
