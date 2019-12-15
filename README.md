# ANSI SGR to HTML

Library to convert limited part of SGR (Select Graphic Rendition) sequence of ANSI escape sequences


## Supported SGR parameters:
* 0 — reset
* 1 — **bold**
* 30–39 — foreground color
{: style="color: blue;" }
* 40–49 — background color
{: style="background-color: lightblue;" }

## Limitations:
* All other SGR parameters are unsupported
* Can only disable SGR parameters in reverse order they where enabled. Terminal emulators can disable them in any order.

## Usage:
TODO

## Compilation:
    mkdir build
    cd build/
    cmake ..
    make