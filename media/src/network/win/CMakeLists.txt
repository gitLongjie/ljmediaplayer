file(GLOB network_win_code ${CMAKE_CURRENT_LIST_DIR}/*.h ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

set(
    src_network_win_src
    ${network_win_code}
    PARENT_SCOPE
)
