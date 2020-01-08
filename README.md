# ANSI Esc to HTML

Library to convert limited part of SGR (Select Graphic Rendition) sequence of ANSI escape sequences to HTML.

## Supported SGR parameters:
* 0 — Reset
* 1 — Bold, 22 — Normal intensity
* 3	— Italic, 23 — Not italic
* 4	— Underline, 24 — Underline off
* 9 — Crossed-out, 29 — Not crossed out
* 30–39, 90–97 — foreground color
* 40–49, 100–107 — background color

## Limitations:
See more info on [wiki](https://github.com/pozemka/ANSIEsc2HTML/wiki/Limitations)

* All other SGR parameters are unsupported and produce no HTML code.
* Can only disable SGR parameters in reverse order they where enabled. Terminal emulators can disable them in any order.

## Usage:
1. include `src/ansi_esc2html.h`
2. either 
   * add `src/ansi_esc2html.cpp` to your project
   * or link with `ansi_esc2html` library
3. Example:
```cpp
ANSI_SGR2HTML ansisgr2html;
std::string ansi = "\x1b[48;5;141m background color \x1b[49m";
std::string html = ansisgr2html.simpleParse(ansi);
```
result HTML will be:
```html
<body style="background-color:#111111;font-family:'Consolas','Droid Sans Mono',monospace; color:#eeeeee; white-space:pre"><span style="background-color:#af87ff"> background color </span></body>
```

## Compilation:
This will produce, release versions of static and dynamic libraries and test executable:

    mkdir build
    cd build/
    cmake -DCMAKE_BUILD_TYPE=Release ..
      # or for Windows
    cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release .. 
    make


