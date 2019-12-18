cmake_minimum_required(VERSION 3.10)

project(ANSI_SGR2HTML VERSION 0.1 DESCRIPTION "Basic ANSI Select Graphic Rendition Control Sequence to HTML library.")

set(LIBRARY_SRCS
#     src/ansi_sgr2html.h
    src/ansi_sgr2html.cpp
)

add_library(ANSI_SGR2HTML SHARED
    "${LIBRARY_SRCS}"
)

add_library(ANSI_SGR2HTML_static STATIC
    "${LIBRARY_SRCS}"
)

set_target_properties(ANSI_SGR2HTML PROPERTIES 
    PUBLIC_HEADER src/ansi_sgr2html.h
    VERSION ${PROJECT_VERSION}
    SOVERSION 0
)
