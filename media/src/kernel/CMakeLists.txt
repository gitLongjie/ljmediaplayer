file(GLOB kernel_code ${CMAKE_CURRENT_LIST_DIR}/*.h ${CMAKE_CURRENT_LIST_DIR}/*.cpp)

set(
    src_kernel_src
    ${kernel_code}
    PARENT_SCOPE
)
