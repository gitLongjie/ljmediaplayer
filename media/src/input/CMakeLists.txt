file(GLOB input_code ${CMAKE_CURRENT_LIST_DIR}/*.h ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

add_subdirectory(rtmp)

set(
    input_src
    ${input_code}
    ${src_rtmp_src}
    PARENT_SCOPE
)
