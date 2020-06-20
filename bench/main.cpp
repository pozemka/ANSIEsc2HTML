#include "../src/ansi_esc2html.h"

#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


using namespace std;

int main()
{
    cout << "ANSIEsc2HTML becnhmark" << endl;

    std::ifstream t("learn.ansi");
    std::stringstream buffer;
    buffer << t.rdbuf();
    string data = buffer.str();
//    data = "24-bit-color: 38 255 85 127 pink 🐨 coala Приве́т नमस्ते שָׁלוֹם";

    const int test_loops = 10000;
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;
    std::string out;
    ANSI_SGR2HTML a;

    auto prepareForBecnhmark = [&out](){
        auto osize = out.size();
        out.clear();
        out.reserve(osize);
    };

    // dry run to determine out capcaity
    out = a.simpleParse(data);


    long accumulated_time = 0;
    for(int loops=0; loops<test_loops; loops++) {
        prepareForBecnhmark();
        begin = std::chrono::steady_clock::now();
        out = a.simpleParse(data);
        end = std::chrono::steady_clock::now();
        accumulated_time += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        std::cout << out[out.size()-1];
    }
//    std::cout << out << "\n";
    long simple_time = accumulated_time/test_loops;

    // dry run to determine out capcaity
    out = a.strictParse(data);
    accumulated_time = 0;
    for(int loops=0; loops<test_loops; loops++) {
        prepareForBecnhmark();
        begin = std::chrono::steady_clock::now();
        out = a.strictParse(data);
        end = std::chrono::steady_clock::now();
        accumulated_time += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        std::cout << out[out.size()-1];
    }
//    std::cout << out << "\n";
    long strict_time = accumulated_time/test_loops;

    std::cout << "\naverage simple = " << simple_time << "[µs]\n";
    std::cout << "\naverage strict = " << strict_time << "[µs]\n";

    return 0;
}
