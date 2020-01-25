#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../src/ansi_esc2html.h"

//TODO: test cr, lf, crlf, etc.
//TODO: test for CSI different from SGR. Make them ignored in simpleParse

static std::string body_start=R"(<body style="background-color:#111111;font-family:'Consolas','Droid Sans Mono',monospace; color:#eeeeee; white-space:pre">)";
static std::string body_end=R"(</body>)";
TEST_CASE( "empty strings", "[empty]" ) {
    ANSI_SGR2HTML a2h;
    SECTION( "empty string" ) {
        std::string res = a2h.simpleParse("");
        CHECK(body_start+body_end == res);
    }
    SECTION( "null string" ) {
        std::string res = a2h.simpleParse(std::string());
        CHECK(body_start+body_end == res);
    }
    SECTION( "unsupported params" ) {
        // ideogram attributes are not supported
        std::string res = a2h.simpleParse("\x1b[62m\x1b[65m");
        CHECK(body_start+body_end == res);
    }
}

TEST_CASE( "colors", "[colors]" ) {
    ANSI_SGR2HTML a2h;
    SECTION( "basic-color: 43 yellow backgorund" ) {
        std::string res = a2h.simpleParse("\x1b[43m yellow background \x1b[49m");
        CHECK(body_start+R"(<span style="background-color:#ffc706"> yellow background </span>)"+body_end == res);
    }
    SECTION( "basic-color: 103 bright-yellow backgorund" ) {
        std::string res = a2h.simpleParse("\x1b[103m bright-yellow background \x1b[49m");
        CHECK(body_start+R"(<span style="background-color:#ffff00"> bright-yellow background </span>)"+body_end == res);
    }
    SECTION( "256-color 48 5 141 blue-pink backgorund" ) {
        std::string res = a2h.simpleParse("\x1b[48;5;141m background color \x1b[49m");
        CHECK(body_start+R"(<span style="background-color:#af87ff"> background color </span>)"+body_end == res);
    }
    SECTION( "24-bit-color: 48 245 222 179 wheat backgorund" ) {
        std::string res = a2h.simpleParse("\x1b[48;2;245;222;179m wheat background \x1b[49m");
        CHECK(body_start+R"(<span style="background-color:#f5deb3"> wheat background </span>)"+body_end == res);
    }

    SECTION( "basic-color: 34 blue foreground" ) {
        std::string res = a2h.simpleParse("\x1b[34m blue foreground \x1b[39m");
        CHECK(body_start+R"(<font color="#006fb8"> blue foreground </font>)"+body_end == res);
    }
    SECTION( "basic-color: 94 bright-blue foreground" ) {
        std::string res = a2h.simpleParse("\x1b[94m bright-blue foreground \x1b[39m");
        CHECK(body_start+R"(<font color="#0000ff"> bright-blue foreground </font>)"+body_end == res);
    }
    SECTION( "256-color 38;5;208 orange foreground foreground" ) {
        std::string res = a2h.simpleParse("\x1b[38;5;208m orange (256 color) foreground \x1b[39m");
        CHECK(body_start+R"(<font color="#ff8700"> orange (256 color) foreground </font>)"+body_end == res);
    }
    SECTION( "24-bit-color: 38 47 79 79 DarkSlateGray foreground" ) {
        std::string res = a2h.simpleParse("\x1b[38;2;47;79;79m DarkSlateGray foreground \x1b[39m");
        CHECK(body_start+R"(<font color="#2f4f4f"> DarkSlateGray foreground </font>)"+body_end == res);
    }

    SECTION( "24-bit-color: 38 255 85 127 pink 🐨 coala" ) {
        std::string res = a2h.simpleParse("\x1b[38;2;255;85;127m pink 🐨 coala \x1b[39m");
        CHECK(body_start+R"(<font color="#ff557f"> pink 🐨 coala </font>)"+body_end == res);
    }
}

TEST_CASE( "formatting", "[format]" ) {
    ANSI_SGR2HTML a2h;
    SECTION( "bold" ) {
        std::string res = a2h.simpleParse("\x1b[1m bold TEXT \x1b[22m");
        CHECK(body_start+R"(<b> bold TEXT </b>)"+body_end == res);
    }
    SECTION( "italic" ) {
        std::string res = a2h.simpleParse("\x1b[3m italic \x1b[23m");
        CHECK(body_start+R"(<i> italic </i>)"+body_end == res);
    }
    SECTION( "underline" ) {
        std::string res = a2h.simpleParse("\x1b[4m underline \x1b[24m");
        CHECK(body_start+R"(<u> underline </u>)"+body_end == res);
    }
    SECTION( "crossed-out" ) {
        std::string res = a2h.simpleParse("\x1b[9m crossed-out \x1b[29m");
        CHECK(body_start+R"(<s> crossed-out </s>)"+body_end == res);
    }
}

TEST_CASE( "mixed", "[mixed]" ) {
    ANSI_SGR2HTML a2h;
    SECTION( "bg+fg colors" ) {
        std::string res = a2h.simpleParse("\x1b[42;36m green bg cyan fg \x1b[39;49m");
        CHECK(body_start+R"(<span style="background-color:#39b54a"><font color="#2cb5e9"> green bg cyan fg </font></span>)"+body_end == res);
    }
    SECTION( "bg+fg+bold" ) {
        std::string res = a2h.simpleParse("\x1b[38;5;208;48;5;141;1m EvErYtHiNg \x1b[0m");
        CHECK(body_start+R"(<font color="#ff8700"><span style="background-color:#af87ff"><b> EvErYtHiNg </b></span></font>)"+body_end == res);
    }
}

TEST_CASE( "complex", "[complex]") {
    ANSI_SGR2HTML a2h;

}

TEST_CASE( "basic bad inputs for simpleParse", "[basic_bad_simple]") {
    ANSI_SGR2HTML a2h;
    SECTION( "Not closed parameter" ) {
        std::string res = a2h.simpleParse("\x1b[48;5;141m background color ");
        CHECK(body_start+R"(<span style="background-color:#af87ff"> background color </span>)"+body_end == res);
    }
    SECTION( "Wrong close parameter" ) {
        std::string res = a2h.simpleParse("\x1b[42m open bg close fg \x1b[39m");
        CHECK(body_start+R"(<span style="background-color:#39b54a"> open bg close fg </span>)"+body_end == res);
    }
    SECTION( "bg+fg colors wrong close orded" ) {
        std::string res = a2h.simpleParse("\x1b[42;36m green bg cyan fg \x1b[49;39m");
        CHECK(body_start+R"(<span style="background-color:#39b54a"><font color="#2cb5e9"> green bg cyan fg </span></font>)"+body_end == res);
    }

    SECTION( "bg+fg+bold then fg+bold" ) {
        std::string res = a2h.simpleParse("\x1b[38;5;208;48;5;141;1m EvErYtHiNg \x1b[49m EvErYtHiNg but background");
        // Wrong close order. As expected
        CHECK(body_start+R"(<font color="#ff8700"><span style="background-color:#af87ff"><b> EvErYtHiNg </span> EvErYtHiNg but background</b></font>)"+body_end == res);
    }
    SECTION( "broken sgrs" ) {
        std::string res = a2h.simpleParse("\x1b[62\x1b[65aaa");
        CHECK(body_start+R"(aaa)"+body_end == res);
    }
    SECTION( "broken sgr then correct one" ) {
        std::string res = a2h.simpleParse("\x1b[31\x1b[32m aaa");
        CHECK(body_start+R"(<font color="#39b54" aaa</font>)"+body_end == res);
    }
}

TEST_CASE( "complex bad inputs for simpleParse", "[complex_bad_simple]") {
    ANSI_SGR2HTML a2h;
}
