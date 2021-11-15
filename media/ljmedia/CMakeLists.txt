file(GLOB ljmedia_code ${CMAKE_CURRENT_LIST_DIR}/*.h ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

set(
    src_ljmedia_src
    ${ljmedia_code}
    PARENT_SCOPE
)
