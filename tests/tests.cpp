#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../src/ansi_esc2html.h"


static std::string body_start=R"(<body style="background-color:#111111;font-family:'Consolas','Droid Sans Mono',monospace; color:#eeeeee; white-space:pre">)";
static std::string body_end=R"(</body>)";
TEST_CASE( "valid input for simpleParse", "[simple]" ) {
    ANSI_SGR2HTML a2h;
    SECTION( "empty string" ) {
        std::string res = a2h.simpleParse("");
        REQUIRE(body_start+body_end == res);
    }
    SECTION( "null string" ) {
        std::string res = a2h.simpleParse(std::string());
        REQUIRE(body_start+body_end == res);
    }
    SECTION( "48 5 141 blue-pink backgorund" ) {
        std::string res = a2h.simpleParse(std::string("\x1b[48;5;141m background color \x1b[49m"));
        REQUIRE(body_start+R"(<span style="background-color:#af87ff"> background color </span>)"+body_end == res);
    }
}

TEST_CASE( "bad input for simpleParse", "[bad_simple]") {
    ANSI_SGR2HTML a2h;
    SECTION( "Not closed parameter" ) {
        std::string res = a2h.simpleParse(std::string("\x1b[48;5;141m background color "));
        REQUIRE(body_start+R"(<span style="background-color:#af87ff"> background color </span>)"+body_end == res);
    }
}
