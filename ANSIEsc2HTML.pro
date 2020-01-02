# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

TARGET = ANSIEsc2HTML_static

TEMPLATE = lib
CONFIG *= staticlib

unix {
    QMAKE_CXXFLAGS *= -std=c++17
}

CONFIG *= c++17

DESTDIR = $$PWD/build

HEADERS = \
   $$PWD/src/ansi_esc2html.h

SOURCES = \
   $$PWD/src/ansi_esc2html.cpp

INCLUDEPATH = \
    $$PWD/src

#DEFINES = 

