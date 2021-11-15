cmake_minimum_required(VERSION 3.10.2)

project(ljmedia VERSION 1.0)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14") # set C++ 11
# set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -std=c99")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    message( STATUS "The comipler ${CMAKE_CXX_COMIPLER} has no C++ 11 suport. Please use a different C++ comipler.")
endif()

set(thirdparty_dir "" CACHE PATH "thirdparty")
if (NOT thirdparty_dir)
    message(thirdparty_dir "please set thirdparty dir !")
endif (NOT thirdparty_dir)

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_BINARY_DIR}
    ${thirdparty_dir}
)

link_directories(
    ${CMAKE_BINARY_DIR} 
)

add_definitions(-DLJ_MEDIA_EXPORT )

add_subdirectory(src)
add_subdirectory(ljmedia)

function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

function(my_add_executable)
	foreach(_source IN ITEMS ${ARGN})
		assign_source_group(${_source})
	endforeach()
	add_executable(${ARGV})
endfunction(my_add_executable)

function(my_add_library)
	foreach(_source IN ITEMS ${ARGN})
		assign_source_group(${_source})
	endforeach()
	ADD_LIBRARY(${ARGV})
endfunction(my_add_library)


my_add_library(
    ${PROJECT_NAME}
    SHARED
    ${src_src}
    ${src_ljmedia_src}
)
