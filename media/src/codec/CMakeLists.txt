file(GLOB codec_code ${CMAKE_CURRENT_LIST_DIR}/*.h ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

set(
    src_codec_src
    ${codec_code}
    PARENT_SCOPE
)
