cmake_minimum_required(VERSION 3.0)

add_executable(bench main.cpp)

target_link_libraries(bench -L${PROJECT_SOURCE_DIR}/../build)
target_link_libraries(bench ANSIEsc2HTML_static)



