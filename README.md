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
TODO

## Compilation:
    mkdir build
    cd build/
    cmake ..
    make