include_directories( ${CMAKE_CURRENT_BINARY_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_llvm_library(LLVMOR1KDisassembler
  OR1KDisassembler.cpp
  )

# workaround for hanging compilation on MSVC9 and 10
if( MSVC_VERSION EQUAL 1400 OR MSVC_VERSION EQUAL 1500 OR MSVC_VERSION EQUAL 1600 )
set_property(
  SOURCE OR1KDisassembler.cpp
  PROPERTY COMPILE_FLAGS "/Od"
  )
endif()

add_dependencies(LLVMOR1KDisassembler OR1KCommonTableGen)
