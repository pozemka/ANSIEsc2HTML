cmake_minimum_required(VERSION 3.21)

project(ANSI_SGR2HTML VERSION 1.0.0 DESCRIPTION "Basic ANSI Select Graphic Rendition Control Sequence to HTML library.")

if(PROJECT_IS_TOP_LEVEL)
    option(ANSI_SGR2HTML_BUILD_TESTS "Build tests" ON)
else()
    option(ANSI_SGR2HTML_BUILD_TESTS "Build tests" OFF)
endif()

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

#target_compile_definitions(tests PRIVATE CATCH_CONFIG_ENABLE_BENCHMARKING)
#set(RELEASE_OPTIONS
#    -O3
#)
#target_compile_options(ANSIEsc2HTML_static PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_OPTIONS}>")
#target_compile_options(tests PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_OPTIONS}>")

set_target_properties(ANSIEsc2HTML ANSIEsc2HTML_static PROPERTIES
    PUBLIC_HEADER src/ansi_esc2html.h
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)

if(ANSI_SGR2HTML_BUILD_TESTS)
    add_executable(tests
        tests/tests.cpp
    )
    target_link_libraries(tests PRIVATE ANSIEsc2HTML_static)

    set_target_properties(tests PROPERTIES
        CXX_STANDARD 17
        CXX_EXTENSIONS OFF
    )

    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    include(CTest)
    include(Catch)
    catch_discover_tests(tests)
endif()
