file(GLOB network_code ${CMAKE_CURRENT_LIST_DIR}/*.h ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

set(
    src_network_src
    ${network_code}
    PARENT_SCOPE
)
