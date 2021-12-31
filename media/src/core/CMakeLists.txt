file(GLOB core_code ${CMAKE_CURRENT_LIST_DIR}/*.h ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

set(
    src_core_src
    ${core_code}
    PARENT_SCOPE
)
