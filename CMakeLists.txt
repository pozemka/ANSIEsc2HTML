cmake_minimum_required(VERSION 3.10)

project(ANSI_SGR2HTML VERSION 1.0.0 DESCRIPTION "Basic ANSI Select Graphic Rendition Control Sequence to HTML library.")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(LIBRARY_SRCS
#     src/ansi_sgr2html.h
    src/ansi_esc2html.cpp
)

add_library(ANSIEsc2HTML SHARED
    "${LIBRARY_SRCS}"
)

add_library(ANSIEsc2HTML_static STATIC
    "${LIBRARY_SRCS}"
)

add_executable(tests
    EXCLUDE_FROM_ALL
    tests/catch.hpp
    tests/tests.cpp
)

target_link_libraries(tests ANSIEsc2HTML_static)

#target_compile_definitions(tests PRIVATE CATCH_CONFIG_ENABLE_BENCHMARKING)
#set(RELEASE_OPTIONS
#    -O3
#)
#target_compile_options(ANSIEsc2HTML_static PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_OPTIONS}>")
#target_compile_options(tests PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_OPTIONS}>")

add_dependencies(tests ANSIEsc2HTML_static)

set_target_properties(ANSIEsc2HTML ANSIEsc2HTML_static tests PROPERTIES
    PUBLIC_HEADER src/ansi_esc2html.h
#     VERSION ${PROJECT_VERSION}
#     SOVERSION 0
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)

# locate Catch2 scripts for CMake
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/tests)
include(CTest)
include(Catch)

# register Catch2 tests as individual CTest tests
catch_discover_tests(tests)
