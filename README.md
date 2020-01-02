# ANSI Esc to HTML

Library to convert limited part of SGR (Select Graphic Rendition) sequence of ANSI escape sequences to HTML.

## Supported SGR parameters:
* 0 — Reset
* 1 — Bold, 22 — Normal intensity
* 3	— Italic, 23 — Not italic
* 4	— Underline, 24 — Underline off
* 9 — Crossed-out, 29 — Not crossed out
* 30–39 — foreground color
* 40–49 — background color

## Limitations:
* All other SGR parameters are unsupported.
* Can only disable SGR parameters in reverse order they where enabled. Terminal emulators can disable them in any order.

## Usage:
1. include `src/ansi_esc2html.h`
2. either 
2. * add `src/ansi_esc2html.cpp` to your project
2. * or link with `ansi_esc2html` library
3. Example:
```cpp
ANSI_SGR2HTML ansisgr2html;
std::string ansi = "[38;5;246m Hello World! [39m";
std::string html = ansisgr2html.simpleParse(ansi);
```

## Compilation:
    mkdir build
    cd build/
    cmake ..
    make

or

    qmake
    make release