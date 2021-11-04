file(GLOB input_code ${CMAKE_CURRENT_LIST_DIR}/*.h ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

set(
    input_src
    ${input_code}
    PARENT_SCOPE
)
