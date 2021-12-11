file(GLOB rtmp_code ${CMAKE_CURRENT_LIST_DIR}/*.h ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

set(
    src_rtmp_src
    ${rtmp_code}
    PARENT_SCOPE
)
