file(GLOB src_code ${CMAKE_CURRENT_LIST_DIR}/*.h ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

add_subdirectory(input)

set(
    src_src
    ${src_code}
    ${input_src}
    PARENT_SCOPE
)
