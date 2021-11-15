file(GLOB network_code ${CMAKE_CURRENT_LIST_DIR}/*.h ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

if (WIN32)
add_subdirectory(win)
set(
    src_plane_src
    ${src_network_win_src}
)
endif ()

set(
    src_network_src
    ${network_code}
    ${src_plane_src}
    PARENT_SCOPE
)
