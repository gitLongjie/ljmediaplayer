cmake_minimum_required(VERSION 3.10.2)

project(ljmedia_mac VERSION 1.0)

add_compile_options(-x objective-c++)
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_EXE_LINKER_FLAGS "-framework Cocoa -framework AppKit -framework CoreData -framework Foundation")

file(GLOB src_code ${CMAKE_CURRENT_LIST_DIR}/*.h ${CMAKE_CURRENT_LIST_DIR}/*.m)

macro (append_files files ext)
  foreach (dir ${ARGN})
    file (GLOB _files "${dir}/*.${ext}")
    list (APPEND ${files} ${_files})
  endforeach (dir)
endmacro (append_files)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

append_files (Headers "h" .)
append_files (Sources "cpp" .)

add_executable (ljmedia_mac ${Headers} ${Templates} ${Sources} ${src_code})
target_link_libraries (ljmedia_mac ${TESTOMP_LIBRARIES})

if (CMAKE_GENERATOR STREQUAL Xcode)
	set_target_properties(ljmedia_mac PROPERTIES XCODE_ATTRIBUTE_CC ${LLVM_ROOT_DIR}/bin/clang)
	set_target_properties(ljmedia_mac PROPERTIES XCODE_ATTRIBUTE_CXX ${LLVM_ROOT_DIR}/bin/clang)
	set_target_properties(ljmedia_mac PROPERTIES XCODE_ATTRIBUTE_COMPILER_INDEX_STORE_ENABLE "No")
endif()

