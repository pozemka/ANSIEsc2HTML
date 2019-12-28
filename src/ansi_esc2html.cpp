/*
 * Copyright (c) 2019 Mikhail Bryukhovets
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "ansi_esc2html.h"

#include <deque>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <type_traits>
#include <unordered_map>

class ANSI_SGR2HTML::impl
{
    typedef std::deque<unsigned char> SGRParts;
public:
    std::string simpleParse(const std::string& raw_data);
    
private:
    SGRParts splitSGR(const std::string& data);
    std::string processSGR(SGRParts& sgr_parts/*non const!*/);
    std::string detectHTMLSymbol(char symbol);
    std::string getHexStr(unsigned int num);
    void resetAll(std::string& out);
    // template will help replace strings and chars with their wide counterparts
    template<typename T, typename U> void resetAttribute(T& attribute_stack, std::basic_string<U>& out);
    const char* decodeColor256(unsigned char color_code);
    const char* decodeColorBasic(unsigned char color_code);
    std::stack<const char*> stack_intensity_;
    std::stack<const char*> stack_italic_;
    std::stack<const char*> stack_underline_;
    std::stack<const char*> stack_cross_out_;
    std::stack<const char*> stack_fg_color_;
    std::stack<const char*> stack_bg_color_;
    //can't constexpr maps so other trick used
    static const std::unordered_map<unsigned char, const char*> colors_basic_;
};


std::string ANSI_SGR2HTML::impl::simpleParse(const std::string &raw_data)
{
    std::string out_s;
    std::string param_bytes_buf;
    out_s.reserve(raw_data.size());
    // в итоге шрифт и фон должен настраиваться же и браться из редактора!
    //NOTE: Use apostrophes ' not quotes " inside style quotation marks!
    out_s.append(R"(<body style="background-color:#111111;font-family:'Consolas','Droid Sans Mono',monospace; color:#eeeeee; white-space:pre">)");
    size_t i = 0;
    bool ESC = false;
    bool CSI = false;
    do {
        //TODO: можно сэкономить на копировании ? или не стоит
        char c = raw_data[i];
        if (0x1B == c) {
            ESC = true;
            continue;
          }
        if (0x5B == c) {                                    // [
            if (ESC) {
                CSI = true;
              } else {
                  out_s.append(detectHTMLSymbol(c));
                }
                continue;
          }
        if ('m' == c) {
            if (CSI && ESC) {                               // конец ESC-SGR последовательности
                auto sgr = splitSGR(param_bytes_buf);
                out_s.append(processSGR(sgr));
                param_bytes_buf.clear();
                CSI = ESC = false;                          // конец ESC
              } else {
                  out_s.append(detectHTMLSymbol(c));
                }
                continue;
          }
        if (0x30 <= c && 0x3F >= c) {                       // наполнение SGR
            if (CSI && ESC) {
                //                qDebug("param byte 0x%02x", c);
                param_bytes_buf.push_back(c);
              } else {
                  out_s.append(detectHTMLSymbol(c));
                }
                continue;
          }
        if (0x0a == c) {                                    // LF
            //TODO: а что насчёт CR и CRLF?
            out_s.append("<br />");
            continue;
          }
        out_s.append(detectHTMLSymbol(c));                  //default
      } while (i++ < raw_data.size());
    resetAll(out_s);                                        //closes remaining tags
    out_s.append("</body>");
    return out_s;
}


ANSI_SGR2HTML::impl::SGRParts ANSI_SGR2HTML::impl::splitSGR(const std::string& data)
{
    SGRParts sgr_parts;
    std::stringstream split_buf;
    unsigned int sgr_param = 0;                             // If char used here stringstream >> extract symbols not as numbers, so used int
    for(size_t i = 0; i < data.size(); ++i) {
        const char* c = &data[i];                           // экономим на спичках :)
        if(';' == *c) {                                     // разделитель
            split_buf >> sgr_param;
            // charconv (C++17) будет менее удобен т.к. наполняется по одному символу. то же с to_string()
            sgr_parts.push_back(static_cast<unsigned char>(sgr_param));
            split_buf.clear();                              // unsets failbit if any
          } else {
              split_buf << *c;
            }
      }
    split_buf >> sgr_param;
    sgr_parts.push_back(static_cast<unsigned char>(sgr_param));
    return sgr_parts;
}


std::string ANSI_SGR2HTML::impl::processSGR(SGRParts& sgr_parts/*non const!*/)
{
    std::string out;                                        // наверное, нет смысла резервировать размер. ОН будет меняться лишь 1-3 раза за функцию.

    if (sgr_parts.empty())
    return out;                                         //нечего парсить!
    unsigned char sgr_code = sgr_parts[0];

    switch (sgr_code) {
        case 0:                                                 // Reset / Normal	all attributes off
        resetAll(out);
            break;

        case 1:                                                 // Bold or increased intensity
        out.append("<b>");
        stack_intensity_.push("</b>");
            break;
        case 3:                                                 // Italic
        out.append("<i>");
        stack_italic_.push("</i>");
            break;
        case 4:                                                 // Underline
        out.append("<u>");
        stack_underline_.push("</u>");
            break;
        case 9:                                                 // Crossed-out
        out.append("<s>");
        stack_cross_out_.push("</s>");
            break;
        case 22:                                                // Normal color or intensity
        resetAttribute(stack_intensity_, out);
            break;
        case 23:                                                // Not italic, not Fraktur
        resetAttribute(stack_italic_, out);
            break;
        case 24:                                                // Underline off
        resetAttribute(stack_underline_, out);
            break;
        case 29:                                                // Not crossed out
        resetAttribute(stack_cross_out_, out);
            break;
        case 39:                                                // Default foreground color
        resetAttribute(stack_fg_color_, out);
            break;
        case 49:                                                // Default background color
        resetAttribute(stack_bg_color_, out);
            break;
        case 38:                                                // Set foreground color
        if (5 == sgr_parts[1] && sgr_parts.size() >= 3) {   // 8-bit foreground color // 38:5:⟨n⟩
            out.append(R"(<font color=")");
            out.append(decodeColor256(sgr_parts[2]));
            out.append(R"(">)");
            sgr_parts.pop_front();
            sgr_parts.pop_front();
            sgr_parts.pop_front();
            stack_fg_color_.push("</font>");
          // 24-bit foreground color //38;2;⟨r⟩;⟨g⟩;⟨b⟩
          } else if (2 == sgr_parts[1] && sgr_parts.size() >= 5) {
              out.append(R"(<font color="#)");
              out.append(getHexStr(sgr_parts[2]));
              out.append(getHexStr(sgr_parts[3]));
              out.append(getHexStr(sgr_parts[4]));
              out.append(R"(">)");
              sgr_parts.pop_front();
              sgr_parts.pop_front();
              sgr_parts.pop_front();
              sgr_parts.pop_front();
              sgr_parts.pop_front();
              stack_fg_color_.push("</font>");
            } else {
                return out;
              }
            break;
        case 48:                                                // Set background color
        if (5 == sgr_parts[1] && sgr_parts.size() >= 3) {   // 8-bit background color // 48:5:⟨n⟩
            out.append(R"(<span style="background-color:)");
            out.append(decodeColor256(sgr_parts[2]));
            out.append(R"(">)");
            sgr_parts.pop_front();
            sgr_parts.pop_front();
            sgr_parts.pop_front();
            stack_bg_color_.push("</span>");
          // 24-bit background color //48;2;⟨r⟩;⟨g⟩;⟨b⟩
          } else if (2 == sgr_parts[1] && sgr_parts.size() >= 5) {
              out.append(R"(<span style="background-color:#)");
              out.append(getHexStr(sgr_parts[2]));
              out.append(getHexStr(sgr_parts[3]));
              out.append(getHexStr(sgr_parts[4]));
              out.append(R"(">)");
              sgr_parts.pop_front();
              sgr_parts.pop_front();
              sgr_parts.pop_front();
              sgr_parts.pop_front();
              sgr_parts.pop_front();
              stack_bg_color_.push("</span>");
            } else {
                return out;
              }
            break;

        default:                                                // SGR code ranges
        if (30 <= sgr_code && 37 >= sgr_code) {             // foreground color from table
            // TODO: возможно вернуть <font color> если будет две версии парсера и это ничего не сломает.
            out.append(R"(<font color=")");                 // TODO: как-то красивее конструировать строку. Можно использовать {fmt} или подождать С++20 с eel.is/c++draft/format. Пока сойдёт так.
            out.append(decodeColorBasic(sgr_code));
            out.append(R"(">)");
            stack_fg_color_.push("</font>");
          } else if (40 <= sgr_code && 47 >= sgr_code) {      // background color from table
              out.append(R"(<span style="background-color:)");
              out.append(decodeColorBasic(sgr_code));
              out.append(R"(">)");
              stack_bg_color_.push("</span>");
            } else {
                std::cerr << "Warning: unsupported SGR: " <<  sgr_code << std::endl;
              }
      }

    // убираем обработанные параметры
    if (sgr_code < 38                                       // эти параметры состоят из одного параметра
        || (sgr_code >= 39 && sgr_code <= 47)
        || sgr_code >= 49
      ) {
          sgr_parts.pop_front();
        }                                                       // остальные параметры сами знают сколько им убирать (38 может убрать 3 или 5)

    if (sgr_parts.empty())                                  // Параметры кончились
    return out;                                         // Экономим вызов функции

    out += processSGR(sgr_parts);
    return out;
}


std::string ANSI_SGR2HTML::impl::detectHTMLSymbol(char symbol)
{
    switch (symbol) {
    case '"':
        return "&quot;";
    case '\'':
        return "&apos;";
    case '&':
        return "&amp;";
    case '<':
        return "&lt;";
    case '>':
        return "&gt;";
    case '\0':                                              //и так бывает?
        return "";
    default:
        return std::string(&symbol, 1);
    }
}


std::string ANSI_SGR2HTML::impl::getHexStr(unsigned int num)
{
    /*  // too slow?
    std::stringstream color_buf;    // avoid recreating stream every time. See https://stackoverflow.com/a/624291/149897
    color_buf << std::hex << std::setfill('0') << std::setw(2) << num;
    */

    //based on charconv's __to_chars_16. But have leading zero. Original to_chars don't add leading zero
    static constexpr char digits[513] =
            "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"
            "202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f"
            "404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f"
            "606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f"
            "808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9f"
            "a0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebf"
            "c0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedf"
            "e0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfeff";

    std::string str16(2, 0);

    if(num >= 0x100)
        return "ff";    // value out of range

    auto const num2 = num * 2;
    str16[0] = digits[num2];
    str16[1] = digits[num2 + 1];
    return  str16.data();
}


void ANSI_SGR2HTML::impl::resetAll(std::string& out)
{
    resetAttribute(stack_intensity_, out);
    resetAttribute(stack_italic_, out);
    resetAttribute(stack_underline_, out);
    resetAttribute(stack_cross_out_, out);
    resetAttribute(stack_fg_color_, out);
    resetAttribute(stack_bg_color_, out);
}


const char* ANSI_SGR2HTML::impl::decodeColor256(unsigned char color_code)
{
    static constexpr std::array<const char*, 256> colors_256 = {
        "#000000",        // Black
        "#de382b",        // Red
        "#39b54a",        // Green
        "#ffc706",        // Yellow
        "#006fb8",        // Blue
        "#762671",        // Magenta
        "#2cb5e9",        // Cyan
        "#cccccc",        // White
        "#808080",        // Bright Black
        "#ff0000",        // Bright Red
        "#00ff00",        // Bright Green
        "#ffff00",        // Bright Yellow
        "#0000ff",        // Bright Blue
        "#ff00ff",        // Bright Magenta
        "#00ffff",        // Bright Cyan
        "#ffffff",        // Bright White

    //color table generation
    //based on https://unix.stackexchange.com/a/269085
    //16 + 36 × r + 6 × g + b
//     for (unsigned char i = 16; i <= 231; ++i) {
//         int r = (i - 16) / 36;
//         r = !r ? 0 : r * 40 + 55;                           // What are 40 and 55 ?
//         int g = (i - 16) / 6 % 6;
//         g = !g ? 0 : g * 40 + 55;
//         int b = (i - 16) % 6;
//         b = !b ? 0 : b * 40 + 55;
//         QString col = QString("#%1%2%3").arg(r, 2, 16, QLatin1Char('0')).arg(g, 2, 16, QLatin1Char('0')).arg(b, 2, 16, QLatin1Char('0'));
//         if (col != colors_256[i])
//             qFatal("color miss %s %s", colors_256[i], qPrintable(col));
//     }

        "#000000",
        "#00005f",
        "#000087",
        "#0000af",
        "#0000d7",
        "#0000ff",
        "#005f00",
        "#005f5f",
        "#005f87",
        "#005faf",
        "#005fd7",
        "#005fff",
        "#008700",
        "#00875f",
        "#008787",
        "#0087af",
        "#0087d7",
        "#0087ff",
        "#00af00",
        "#00af5f",
        "#00af87",
        "#00afaf",
        "#00afd7",
        "#00afff",
        "#00d700",
        "#00d75f",
        "#00d787",
        "#00d7af",
        "#00d7d7",
        "#00d7ff",
        "#00ff00",
        "#00ff5f",
        "#00ff87",
        "#00ffaf",
        "#00ffd7",
        "#00ffff",
        "#5f0000",
        "#5f005f",
        "#5f0087",
        "#5f00af",
        "#5f00d7",
        "#5f00ff",
        "#5f5f00",
        "#5f5f5f",
        "#5f5f87",
        "#5f5faf",
        "#5f5fd7",
        "#5f5fff",
        "#5f8700",
        "#5f875f",
        "#5f8787",
        "#5f87af",
        "#5f87d7",
        "#5f87ff",
        "#5faf00",
        "#5faf5f",
        "#5faf87",
        "#5fafaf",
        "#5fafd7",
        "#5fafff",
        "#5fd700",
        "#5fd75f",
        "#5fd787",
        "#5fd7af",
        "#5fd7d7",
        "#5fd7ff",
        "#5fff00",
        "#5fff5f",
        "#5fff87",
        "#5fffaf",
        "#5fffd7",
        "#5fffff",
        "#870000",
        "#87005f",
        "#870087",
        "#8700af",
        "#8700d7",
        "#8700ff",
        "#875f00",
        "#875f5f",
        "#875f87",
        "#875faf",
        "#875fd7",
        "#875fff",
        "#878700",
        "#87875f",
        "#878787",
        "#8787af",
        "#8787d7",
        "#8787ff",
        "#87af00",
        "#87af5f",
        "#87af87",
        "#87afaf",
        "#87afd7",
        "#87afff",
        "#87d700",
        "#87d75f",
        "#87d787",
        "#87d7af",
        "#87d7d7",
        "#87d7ff",
        "#87ff00",
        "#87ff5f",
        "#87ff87",
        "#87ffaf",
        "#87ffd7",
        "#87ffff",
        "#af0000",
        "#af005f",
        "#af0087",
        "#af00af",
        "#af00d7",
        "#af00ff",
        "#af5f00",
        "#af5f5f",
        "#af5f87",
        "#af5faf",
        "#af5fd7",
        "#af5fff",
        "#af8700",
        "#af875f",
        "#af8787",
        "#af87af",
        "#af87d7",
        "#af87ff",
        "#afaf00",
        "#afaf5f",
        "#afaf87",
        "#afafaf",
        "#afafd7",
        "#afafff",
        "#afd700",
        "#afd75f",
        "#afd787",
        "#afd7af",
        "#afd7d7",
        "#afd7ff",
        "#afff00",
        "#afff5f",
        "#afff87",
        "#afffaf",
        "#afffd7",
        "#afffff",
        "#d70000",
        "#d7005f",
        "#d70087",
        "#d700af",
        "#d700d7",
        "#d700ff",
        "#d75f00",
        "#d75f5f",
        "#d75f87",
        "#d75faf",
        "#d75fd7",
        "#d75fff",
        "#d78700",
        "#d7875f",
        "#d78787",
        "#d787af",
        "#d787d7",
        "#d787ff",
        "#d7af00",
        "#d7af5f",
        "#d7af87",
        "#d7afaf",
        "#d7afd7",
        "#d7afff",
        "#d7d700",
        "#d7d75f",
        "#d7d787",
        "#d7d7af",
        "#d7d7d7",
        "#d7d7ff",
        "#d7ff00",
        "#d7ff5f",
        "#d7ff87",
        "#d7ffaf",
        "#d7ffd7",
        "#d7ffff",
        "#ff0000",
        "#ff005f",
        "#ff0087",
        "#ff00af",
        "#ff00d7",
        "#ff00ff",
        "#ff5f00",
        "#ff5f5f",
        "#ff5f87",
        "#ff5faf",
        "#ff5fd7",
        "#ff5fff",
        "#ff8700",
        "#ff875f",
        "#ff8787",
        "#ff87af",
        "#ff87d7",
        "#ff87ff",
        "#ffaf00",
        "#ffaf5f",
        "#ffaf87",
        "#ffafaf",
        "#ffafd7",
        "#ffafff",
        "#ffd700",
        "#ffd75f",
        "#ffd787",
        "#ffd7af",
        "#ffd7d7",
        "#ffd7ff",
        "#ffff00",
        "#ffff5f",
        "#ffff87",
        "#ffffaf",
        "#ffffd7",
        "#ffffff",

        //grays
        "#080808",
        "#121212",
        "#1c1c1c",
        "#262626",
        "#303030",
        "#3a3a3a",
        "#444444",
        "#4e4e4e",
        "#585858",
        "#626262",
        "#6c6c6c",
        "#767676",
        "#808080",
        "#8a8a8a",
        "#949494",
        "#9e9e9e",
        "#a8a8a8",
        "#b2b2b2",
        "#bcbcbc",
        "#c6c6c6",
        "#d0d0d0",
        "#dadada",
        "#e4e4e4",
        "#eeeeee"
    };
    return colors_256[color_code];
}


//Not sure if maps will be optimized as inline if defined statically inside function scope so defined in class scope
const std::unordered_map<unsigned char, const char*> ANSI_SGR2HTML::impl::colors_basic_ = {
    {30,  "#000000"},
    {31,  "#de382b"},
    {32,  "#39b54a"},
    {33,  "#ffc706"},
    {34,  "#006fb8"},
    {35,  "#762671"},
    {36,  "#2cb5e9"},
    {37,  "#cccccc"},
    {90,  "#808080"},
    {91,  "#ff0000"},
    {92,  "#00ff00"},
    {93,  "#ffff00"},
    {94,  "#0000ff"},
    {95,  "#ff00ff"},
    {96,  "#00ffff"},
    {97,  "#ffffff"},
    //same colors
    {40,  "#000000"},
    {41,  "#de382b"},
    {42,  "#39b54a"},
    {43,  "#ffc706"},
    {44,  "#006fb8"},
    {45,  "#762671"},
    {46,  "#2cb5e9"},
    {47,  "#cccccc"},
    {100, "#808080"},
    {101, "#ff0000"},
    {102, "#00ff00"},
    {103, "#ffff00"},
    {104, "#0000ff"},
    {105, "#ff00ff"},
    {106, "#00ffff"},
    {107, "#ffffff"},
  };                                                          // static defenition


const char* ANSI_SGR2HTML::impl::decodeColorBasic(unsigned char color_code)
{
    if(colors_basic_.count(color_code))
        return colors_basic_.at(color_code);
    return "#ffffff";
}


template<typename T, typename U>
void ANSI_SGR2HTML::impl::resetAttribute(T& attribute_stack, std::basic_string<U>& out)
{
    static_assert (std::is_same_v<T, std::stack<const U*> >, "T must be std::stack of const CharT*");
    while (!attribute_stack.empty()) {
        out.append(attribute_stack.top());
        attribute_stack.pop();
    }
}


// ANSI_SGR2HTML
ANSI_SGR2HTML::ANSI_SGR2HTML() :
    pimpl_(std::make_unique<impl>())
{

}

ANSI_SGR2HTML::~ANSI_SGR2HTML()
{

}

std::string ANSI_SGR2HTML::simpleParse(const std::string &raw_data)
{
    return pimpl_->simpleParse(raw_data);
}
