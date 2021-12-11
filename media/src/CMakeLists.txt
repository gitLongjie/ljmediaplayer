file(GLOB src_code ${CMAKE_CURRENT_LIST_DIR}/*.h ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

add_subdirectory(input)
add_subdirectory(ljmedia)
add_subdirectory(network)
add_subdirectory(codec)

set(
    src_src
    ${src_code}
    ${src_input_src}
    ${src_ljmedia_src}
    ${src_network_src}
    ${src_codec_src}
    PARENT_SCOPE
)
