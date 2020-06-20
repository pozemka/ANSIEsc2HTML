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
See more info on [wiki](https://github.com/pozemka/ANSIEsc2HTML/wiki/Limitations).

* All other SGR parameters are unsupported and produce no HTML code.

## Usage:
1. include `src/ansi_esc2html.h`
2. either
   * add `src/ansi_esc2html.cpp` to your project
   * or link against `ansi_esc2html` library
3. Example:
```cpp
ANSI_SGR2HTML ansisgr2html;
std::string ansi = "\x1b[48;5;141m background color \x1b[49m";
std::string html = ansisgr2html.simpleParse(ansi);
//or
std::string html_strict = ansisgr2html.strictParse(ansi);
```
result HTML will be:
```html
<body style="background-color:#111111;font-family:'Consolas','Droid Sans Mono',monospace; color:#eeeeee; white-space:pre"><span style="background-color:#af87ff"> background color </span></body>
```

## Functions:
### simpleParse
Can only disable SGR parameters in reverse order they where enabled. Terminal emulators can disable them in any order. See example on [wiki](https://github.com/pozemka/ANSIEsc2HTML/wiki/Limitations)

This function is suitable for ANSI code generated from markup languages like XML or Markdown.

### strictParse
Cans disable SGR parameters in any order. Same as terminal emulators. But it is ~40% slower than simpleParse.

## Performance:
Tested on:

* Intel® Core™ i5-3570K (4 GHz overclocked).
* Input text 101&nbsp;552 bytes. More then half is SGR data
* You can see bechmark code and data [here](https://github.com/pozemka/ANSIEsc2HTML/tree/master/bench)

Results:

* `simpleParse` ~ 1082 µs
* `strictParse` ~ 1400 µs

## Compilation:
This will produce release versions of static and dynamic libraries and test executable:

    mkdir build
    cd build/
    cmake -DCMAKE_BUILD_TYPE=Release ..
      # or for Windows
    cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release .. 
    make
