cmake_minimum_required(VERSION 3.10)

project(ANSI_SGR2HTML VERSION 0.1 DESCRIPTION "Basic ANSI Select Graphic Rendition Control Sequence to HTML library.")

add_library(ANSI_SGR2HTML SHARED
#     src/ansi_sgr2html.h
    src/ansi_sgr2html.cpp
)

set_target_properties(ANSI_SGR2HTML PROPERTIES 
    PUBLIC_HEADER src/ansi_sgr2html.h
    VERSION ${PROJECT_VERSION}
    SOVERSION 0
)
