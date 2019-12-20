cmake_minimum_required(VERSION 3.10)

project(ANSI_SGR2HTML VERSION 0.1 DESCRIPTION "Basic ANSI Select Graphic Rendition Control Sequence to HTML library.")

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

set_target_properties(ANSIEsc2HTML PROPERTIES 
    PUBLIC_HEADER src/ansi_esc2html.h
    VERSION ${PROJECT_VERSION}
    SOVERSION 0
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)
