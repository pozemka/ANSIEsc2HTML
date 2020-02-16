#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../src/ansi_esc2html.h"

#include <fstream>

//TODO: test cr, lf, crlf, etc.
//TODO: test for CSI different from SGR. Make them ignored in simpleParse

struct AE2HTestCase {
    std::string input;
    std::string expected;
};

static std::string body_start=R"(<body style="background-color:#111111;font-family:'Consolas','Droid Sans Mono',monospace; color:#eeeeee; white-space:pre">)";
static std::string body_end=R"(</body>)";
TEST_CASE( "empty strings", "[empty]" ) {
    ANSI_SGR2HTML a2h;
    SECTION( "empty string" ) {
        std::string res = a2h.simpleParse("");
        CHECK(res == body_start+body_end);
    }
    SECTION( "null string" ) {
        std::string res = a2h.simpleParse(std::string());
        CHECK(res == body_start+body_end);
    }
    SECTION( "empty param" ) {
        std::string res = a2h.simpleParse("\x1b[m");
        CHECK(res == body_start+body_end);
    }
    SECTION( "unsupported params" ) {
        // ideogram attributes are not supported
        std::string res = a2h.simpleParse("\x1b[62m\x1b[65m");
        CHECK(res == body_start+body_end);
    }
}

TEST_CASE( "colors", "[colors]" ) {
    ANSI_SGR2HTML a2h;
    auto [section, test_case] = GENERATE( table<std::string, AE2HTestCase>({
        {"basic-color: 43 yellow backgorund",
            {"\x1b[43m yellow background \x1b[49m",
                R"(<span style="background-color:#ffc706"> yellow background </span>)"}},
        {"basic-color: 103 bright-yellow backgorund",
            {"\x1b[103m bright-yellow background \x1b[49m",
                R"(<span style="background-color:#ffff00"> bright-yellow background </span>)"}},
        {"256-color 48 5 141 blue-pink backgorund",
            {"\x1b[48;5;141m background color \x1b[49m",
                R"(<span style="background-color:#af87ff"> background color </span>)"}},
        {"24-bit-color: 48 245 222 179 wheat backgorund",
            {"\x1b[48;2;245;222;179m wheat background \x1b[49m",
                R"(<span style="background-color:#f5deb3"> wheat background </span>)"}},

        {"basic-color: 34 blue foreground",
            {"\x1b[34m blue foreground \x1b[39m",
                R"(<font color="#006fb8"> blue foreground </font>)"}},
        {"basic-color: 94 bright-blue foreground",
            {"\x1b[94m bright-blue foreground \x1b[39m",
                R"(<font color="#0000ff"> bright-blue foreground </font>)"}},
        {"256-color 38;5;208 orange foreground foreground",
            {"\x1b[38;5;208m orange (256 color) foreground \x1b[39m",
                R"(<font color="#ff8700"> orange (256 color) foreground </font>)"}},
        {"24-bit-color: 38 47 79 79 DarkSlateGray foreground",
            {"\x1b[38;2;47;79;79m DarkSlateGray foreground \x1b[39m",
                R"(<font color="#2f4f4f"> DarkSlateGray foreground </font>)"}}
    }));
    GIVEN( section )
            THEN( "output should be " << test_case.expected ) {
        CHECK(a2h.simpleParse(test_case.input) == body_start+test_case.expected+body_end);
    }
}

TEST_CASE( "formatting", "[format]" ) {
    ANSI_SGR2HTML a2h;
    auto [section, test_case] = GENERATE( table<std::string, AE2HTestCase>({
        {"bold",
            {"\x1b[1m bold TEXT \x1b[22m",
                R"(<b> bold TEXT </b>)"}},
        {"italic",
            {"\x1b[3m italic \x1b[23m",
                R"(<i> italic </i>)"}},
        {"underline",
            {"\x1b[4m underline \x1b[24m",
                R"(<u> underline </u>)"}},
        {"crossed-out",
            {"\x1b[9m crossed-out \x1b[29m",
                R"(<s> crossed-out </s>)"}}
    }));
    GIVEN( section )
            THEN( "output should be " << test_case.expected ) {
        CHECK(a2h.simpleParse(test_case.input) == body_start+test_case.expected+body_end);
    }
}

TEST_CASE( "mixed", "[mixed]" ) {
    ANSI_SGR2HTML a2h;
    auto [section, test_case] = GENERATE( table<std::string, AE2HTestCase>({
        {"bg+fg colors",
            {"\x1b[42;36m green bg cyan fg \x1b[39;49m",
                R"(<span style="background-color:#39b54a"><font color="#2cb5e9"> green bg cyan fg </font></span>)"}},
        {"bg+fg+bold",
            {"\x1b[38;5;208;48;5;141;1m EvErYtHiNg \x1b[0m",
                R"(<font color="#ff8700"><span style="background-color:#af87ff"><b> EvErYtHiNg </b></span></font>)"}},
        {"fg+bg then fg",
            {"\x1b[36;42mE\x1b[35mxit",
                R"(<font color="#2cb5e9"><span style="background-color:#39b54a">E<font color="#762671">xit</font></span></font>)"}},
        {"bg+fg then fg",
            {"\x1b[46;32mE\x1b[35mxit",
                R"(<span style="background-color:#2cb5e9"><font color="#39b54a">E<font color="#762671">xit</font></font></span>)"}},
        {"bold then reset then bold",
            {"\x1b[1m bold \x1b[0m not bold \x1b[1m bold \x1b[0m",
                R"(<b> bold </b> not bold <b> bold </b>)"}}
    }));
    GIVEN( section )
    THEN( "output should be " << test_case.expected ) {
        CHECK(a2h.simpleParse(test_case.input) == body_start+test_case.expected+body_end);
    }
}

TEST_CASE( "complex", "[complex]") {
    ANSI_SGR2HTML a2h;
    auto [section, test_case] = GENERATE( table<std::string, AE2HTestCase>({
        {"24-bit-color: 38 255 85 127 pink 🐨 coala",
            {"\x1b[38;2;255;85;127m pink 🐨 coala \x1b[39m",
            R"(<font color="#ff557f"> pink 🐨 coala </font>)"}},
        {"24-bit-color: 38 255 85 127 pink 🐨 coala",
            {"\x1b[38;2;25;85;127m capry blue Приве́т नमस्ते שָׁלוֹם mixed locales \x1b[39m",
            R"(<font color="#19557f"> capry blue Приве́т नमस्ते שָׁלוֹם mixed locales </font>)"}}
    }));
    GIVEN( section )
    THEN( "output should be " << test_case.expected ) {
        CHECK(a2h.simpleParse(test_case.input) == body_start+test_case.expected+body_end);
    }
}

// NOTE: HTML produced from bad input is valid but not matching input.
TEST_CASE( "basic bad inputs for simpleParse", "[basic_bad_simple]") {
    ANSI_SGR2HTML a2h;
    auto [section, test_case] = GENERATE( table<std::string, AE2HTestCase>({
        {"Not closed parameter",
            {"\x1b[48;5;141m background color ",
                R"(<span style="background-color:#af87ff"> background color </span>)"}},
        {"Wrong close parameter",
            {"\x1b[42m open bg close fg \x1b[39m",
                R"(<span style="background-color:#39b54a"> open bg close fg </span>)"}},
        {"bg+fg colors wrong close order",
            {"\x1b[42;36m green bg cyan fg \x1b[49;39m",
                R"(<span style="background-color:#39b54a"><font color="#2cb5e9"> green bg cyan fg </font></span>)"}},
        {"bg+fg+bold then fg+bold",
            {"\x1b[38;5;208;48;5;141;1m EvErYtHiNg \x1b[49m EvErYtHiNg but background",
                R"(<font color="#ff8700"><span style="background-color:#af87ff"><b> EvErYtHiNg </b> EvErYtHiNg but background</span></font>)"}},

        {"broken sgrs",
            {"\x1b[62\x1b[65aaa",
                R"()"}},
        {"broken sgrs then close then text",
            {"\x1b[62\x1b[65aaambbb",
                R"(bbb)"}},
        {"broken sgr then correct one",
            {"\x1b[31\x1b[32m aaa",
                R"(<font color="#39b54a"> aaa</font>)"}}
    }));
    GIVEN( section )
    THEN( "output should be " << test_case.expected ) {
        CHECK(a2h.simpleParse(test_case.input) == body_start+test_case.expected+body_end);
    }
}

TEST_CASE( "Unsupported CSIs for simpleParse", "[unsupported_csi_simple]") {
    ANSI_SGR2HTML a2h;
    auto [section, test_case] = GENERATE( table<std::string, AE2HTestCase>({
        {"unsupported CSI: 128",
            {"\x1b[128m unsupported \x1b[49m",
                R"( unsupported )"}},
        {"Unsupported CSI n A",
            {"\x1b[9A", ""}},
        {"Unsupported CSI n;m H",
            {"\x1b[1;5H", ""}},
        {"Unsupported CSI n A + bold",
            {"\x1b[9A\x1b[1m bold TEXT \x1b[22m", R"(<b> bold TEXT </b>)"}}
    }));
    GIVEN( section )
    THEN( "output should be " << test_case.expected ) {
        CHECK(a2h.simpleParse(test_case.input) == body_start+test_case.expected+body_end);
    }
}


TEST_CASE( "complex bad inputs for simpleParse", "[complex_bad_simple]") {
    ANSI_SGR2HTML a2h;
}
