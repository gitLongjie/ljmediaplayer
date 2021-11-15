file(GLOB src_code ${CMAKE_CURRENT_LIST_DIR}/*.h ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

add_subdirectory(input)
add_subdirectory(ljmedia)
add_subdirectory(network)

set(
    src_src
    ${src_code}
    ${input_src}
    ${src_ljmedia_src}
    ${src_network_src}
    PARENT_SCOPE
)
