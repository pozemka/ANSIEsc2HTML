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

#include <iostream>
#include <sstream>
#include <vector>
#include <type_traits>


ANSI_SGR2HTML::ANSI_SGR2HTML()
{
    //TODO: брать цвета из тем?
    colors_basic.reserve(32);
    // можно отнимать 10, но вдруг захочется разные оттенки для фона/текста
    colors_basic[30] = colors_basic[40] = "#000000";        // Black
    colors_basic[31] = colors_basic[41] = "#de382b";        // Red
    colors_basic[32] = colors_basic[42] = "#39b54a";        // Green
    colors_basic[33] = colors_basic[43] = "#ffc706";        // Yellow
    colors_basic[34] = colors_basic[44] = "#006fb8";        // Blue
    colors_basic[35] = colors_basic[45] = "#762671";        // Magenta
    colors_basic[36] = colors_basic[46] = "#2cb5e9";        // Cyan
    colors_basic[37] = colors_basic[47] = "#cccccc";        // White
    colors_basic[90] = colors_basic[100]= "#808080";        // Bright Black
    colors_basic[91] = colors_basic[101]= "#ff0000";        // Bright Red
    colors_basic[92] = colors_basic[102]= "#00ff00";        // Bright Green
    colors_basic[93] = colors_basic[103]= "#ffff00";        // Bright Yellow
    colors_basic[94] = colors_basic[104]= "#0000ff";        // Bright Blue
    colors_basic[95] = colors_basic[105]= "#ff00ff";        // Bright Magenta
    colors_basic[96] = colors_basic[106]= "#00ffff";        // Bright Cyan
    colors_basic[97] = colors_basic[107]= "#ffffff";        // Bright White

    colors_256.reserve(256);
    colors_256[0]   = colors_basic[30];
    colors_256[1]   = colors_basic[31];
    colors_256[2]   = colors_basic[32];
    colors_256[3]   = colors_basic[33];
    colors_256[4]   = colors_basic[34];
    colors_256[5]   = colors_basic[35];
    colors_256[6]   = colors_basic[36];
    colors_256[7]   = colors_basic[37];
    colors_256[8]   = colors_basic[90];
    colors_256[9]   = colors_basic[91];
    colors_256[10]  = colors_basic[92];
    colors_256[11]  = colors_basic[93];
    colors_256[12]  = colors_basic[94];
    colors_256[13]  = colors_basic[95];
    colors_256[14]  = colors_basic[96];
    colors_256[15]  = colors_basic[97];
    
    colors_256[16] ="#000000";
    colors_256[17] ="#00005f";
    colors_256[18] ="#000087";
    colors_256[19] ="#0000af";
    colors_256[20] ="#0000d7";
    colors_256[21] ="#0000ff";
    colors_256[22] ="#005f00";
    colors_256[23] ="#005f5f";
    colors_256[24] ="#005f87";
    colors_256[25] ="#005faf";
    colors_256[26] ="#005fd7";
    colors_256[27] ="#005fff";
    colors_256[28] ="#008700";
    colors_256[29] ="#00875f";
    colors_256[30] ="#008787";
    colors_256[31] ="#0087af";
    colors_256[32] ="#0087d7";
    colors_256[33] ="#0087ff";
    colors_256[34] ="#00af00";
    colors_256[35] ="#00af5f";
    colors_256[36] ="#00af87";
    colors_256[37] ="#00afaf";
    colors_256[38] ="#00afd7";
    colors_256[39] ="#00afff";
    colors_256[40] ="#00d700";
    colors_256[41] ="#00d75f";
    colors_256[42] ="#00d787";
    colors_256[43] ="#00d7af";
    colors_256[44] ="#00d7d7";
    colors_256[45] ="#00d7ff";
    colors_256[46] ="#00ff00";
    colors_256[47] ="#00ff5f";
    colors_256[48] ="#00ff87";
    colors_256[49] ="#00ffaf";
    colors_256[50] ="#00ffd7";
    colors_256[51] ="#00ffff";
    colors_256[52] ="#5f0000";
    colors_256[53] ="#5f005f";
    colors_256[54] ="#5f0087";
    colors_256[55] ="#5f00af";
    colors_256[56] ="#5f00d7";
    colors_256[57] ="#5f00ff";
    colors_256[58] ="#5f5f00";
    colors_256[59] ="#5f5f5f";
    colors_256[60] ="#5f5f87";
    colors_256[61] ="#5f5faf";
    colors_256[62] ="#5f5fd7";
    colors_256[63] ="#5f5fff";
    colors_256[64] ="#5f8700";
    colors_256[65] ="#5f875f";
    colors_256[66] ="#5f8787";
    colors_256[67] ="#5f87af";
    colors_256[68] ="#5f87d7";
    colors_256[69] ="#5f87ff";
    colors_256[70] ="#5faf00";
    colors_256[71] ="#5faf5f";
    colors_256[72] ="#5faf87";
    colors_256[73] ="#5fafaf";
    colors_256[74] ="#5fafd7";
    colors_256[75] ="#5fafff";
    colors_256[76] ="#5fd700";
    colors_256[77] ="#5fd75f";
    colors_256[78] ="#5fd787";
    colors_256[79] ="#5fd7af";
    colors_256[80] ="#5fd7d7";
    colors_256[81] ="#5fd7ff";
    colors_256[82] ="#5fff00";
    colors_256[83] ="#5fff5f";
    colors_256[84] ="#5fff87";
    colors_256[85] ="#5fffaf";
    colors_256[86] ="#5fffd7";
    colors_256[87] ="#5fffff";
    colors_256[88] ="#870000";
    colors_256[89] ="#87005f";
    colors_256[90] ="#870087";
    colors_256[91] ="#8700af";
    colors_256[92] ="#8700d7";
    colors_256[93] ="#8700ff";
    colors_256[94] ="#875f00";
    colors_256[95] ="#875f5f";
    colors_256[96] ="#875f87";
    colors_256[97] ="#875faf";
    colors_256[98] ="#875fd7";
    colors_256[99] ="#875fff";
    colors_256[100]="#878700";
    colors_256[101]="#87875f";
    colors_256[102]="#878787";
    colors_256[103]="#8787af";
    colors_256[104]="#8787d7";
    colors_256[105]="#8787ff";
    colors_256[106]="#87af00";
    colors_256[107]="#87af5f";
    colors_256[108]="#87af87";
    colors_256[109]="#87afaf";
    colors_256[110]="#87afd7";
    colors_256[111]="#87afff";
    colors_256[112]="#87d700";
    colors_256[113]="#87d75f";
    colors_256[114]="#87d787";
    colors_256[115]="#87d7af";
    colors_256[116]="#87d7d7";
    colors_256[117]="#87d7ff";
    colors_256[118]="#87ff00";
    colors_256[119]="#87ff5f";
    colors_256[120]="#87ff87";
    colors_256[121]="#87ffaf";
    colors_256[122]="#87ffd7";
    colors_256[123]="#87ffff";
    colors_256[124]="#af0000";
    colors_256[125]="#af005f";
    colors_256[126]="#af0087";
    colors_256[127]="#af00af";
    colors_256[128]="#af00d7";
    colors_256[129]="#af00ff";
    colors_256[130]="#af5f00";
    colors_256[131]="#af5f5f";
    colors_256[132]="#af5f87";
    colors_256[133]="#af5faf";
    colors_256[134]="#af5fd7";
    colors_256[135]="#af5fff";
    colors_256[136]="#af8700";
    colors_256[137]="#af875f";
    colors_256[138]="#af8787";
    colors_256[139]="#af87af";
    colors_256[140]="#af87d7";
    colors_256[141]="#af87ff";
    colors_256[142]="#afaf00";
    colors_256[143]="#afaf5f";
    colors_256[144]="#afaf87";
    colors_256[145]="#afafaf";
    colors_256[146]="#afafd7";
    colors_256[147]="#afafff";
    colors_256[148]="#afd700";
    colors_256[149]="#afd75f";
    colors_256[150]="#afd787";
    colors_256[151]="#afd7af";
    colors_256[152]="#afd7d7";
    colors_256[153]="#afd7ff";
    colors_256[154]="#afff00";
    colors_256[155]="#afff5f";
    colors_256[156]="#afff87";
    colors_256[157]="#afffaf";
    colors_256[158]="#afffd7";
    colors_256[159]="#afffff";
    colors_256[160]="#d70000";
    colors_256[161]="#d7005f";
    colors_256[162]="#d70087";
    colors_256[163]="#d700af";
    colors_256[164]="#d700d7";
    colors_256[165]="#d700ff";
    colors_256[166]="#d75f00";
    colors_256[167]="#d75f5f";
    colors_256[168]="#d75f87";
    colors_256[169]="#d75faf";
    colors_256[170]="#d75fd7";
    colors_256[171]="#d75fff";
    colors_256[172]="#d78700";
    colors_256[173]="#d7875f";
    colors_256[174]="#d78787";
    colors_256[175]="#d787af";
    colors_256[176]="#d787d7";
    colors_256[177]="#d787ff";
    colors_256[178]="#d7af00";
    colors_256[179]="#d7af5f";
    colors_256[180]="#d7af87";
    colors_256[181]="#d7afaf";
    colors_256[182]="#d7afd7";
    colors_256[183]="#d7afff";
    colors_256[184]="#d7d700";
    colors_256[185]="#d7d75f";
    colors_256[186]="#d7d787";
    colors_256[187]="#d7d7af";
    colors_256[188]="#d7d7d7";
    colors_256[189]="#d7d7ff";
    colors_256[190]="#d7ff00";
    colors_256[191]="#d7ff5f";
    colors_256[192]="#d7ff87";
    colors_256[193]="#d7ffaf";
    colors_256[194]="#d7ffd7";
    colors_256[195]="#d7ffff";
    colors_256[196]="#ff0000";
    colors_256[197]="#ff005f";
    colors_256[198]="#ff0087";
    colors_256[199]="#ff00af";
    colors_256[200]="#ff00d7";
    colors_256[201]="#ff00ff";
    colors_256[202]="#ff5f00";
    colors_256[203]="#ff5f5f";
    colors_256[204]="#ff5f87";
    colors_256[205]="#ff5faf";
    colors_256[206]="#ff5fd7";
    colors_256[207]="#ff5fff";
    colors_256[208]="#ff8700";
    colors_256[209]="#ff875f";
    colors_256[210]="#ff8787";
    colors_256[211]="#ff87af";
    colors_256[212]="#ff87d7";
    colors_256[213]="#ff87ff";
    colors_256[214]="#ffaf00";
    colors_256[215]="#ffaf5f";
    colors_256[216]="#ffaf87";
    colors_256[217]="#ffafaf";
    colors_256[218]="#ffafd7";
    colors_256[219]="#ffafff";
    colors_256[220]="#ffd700";
    colors_256[221]="#ffd75f";
    colors_256[222]="#ffd787";
    colors_256[223]="#ffd7af";
    colors_256[224]="#ffd7d7";
    colors_256[225]="#ffd7ff";
    colors_256[226]="#ffff00";
    colors_256[227]="#ffff5f";
    colors_256[228]="#ffff87";
    colors_256[229]="#ffffaf";
    colors_256[230]="#ffffd7";
    colors_256[231]="#ffffff";
 
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
    //grays
    colors_256[232] = "#080808";
    colors_256[233] = "#121212";
    colors_256[234] = "#1c1c1c";
    colors_256[235] = "#262626";
    colors_256[236] = "#303030";
    colors_256[237] = "#3a3a3a";
    colors_256[238] = "#444444";
    colors_256[239] = "#4e4e4e";
    colors_256[240] = "#585858";
    colors_256[241] = "#626262";
    colors_256[242] = "#6c6c6c";
    colors_256[243] = "#767676";
    colors_256[244] = "#808080";
    colors_256[245] = "#8a8a8a";
    colors_256[246] = "#949494";
    colors_256[247] = "#9e9e9e";
    colors_256[248] = "#a8a8a8";
    colors_256[249] = "#b2b2b2";
    colors_256[250] = "#bcbcbc";
    colors_256[251] = "#c6c6c6";
    colors_256[252] = "#d0d0d0";
    colors_256[253] = "#dadada";
    colors_256[254] = "#e4e4e4";
    colors_256[255] = "#eeeeee";
}

ANSI_SGR2HTML::~ANSI_SGR2HTML()
{

}

std::string ANSI_SGR2HTML::simpleParse(std::string raw_data)
{
    std::string out_s;
    std::string param_bytes_buf;
    out_s.reserve(raw_data.size());
    // в итоге шрифт и фон должен настраиваться же и браться из редактора!
    //NOTE: Use apostrophes ' not quotes " inside style quotation marks!
    out_s.append(R"(<body style="background-color:#111111;font-family:'Consolas','Droid Sans Mono',monospace;color:#eeeeee">)");
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

ANSI_SGR2HTML::SGRParts ANSI_SGR2HTML::splitSGR(const std::string& data)
{
    SGRParts sgr_parts;
    std::stringstream split_buf;
    // Если делать чисто для Qt то лучше QByteArray::split. Он вернёт QList<QByteArray>
    unsigned int sgr_param = 0;
    for(size_t i = 0; i < data.size(); ++i) {
        const char* c = &data[i];                           // экономим на спичках :)
        if(';' == *c) {                                     // разделитель
            split_buf >> sgr_param;
            sgr_parts.push_back(sgr_param);                 // charconv (C++17) будет менее удобен т.к. наполняется по одному символу. то же с to_string()
            split_buf.clear();                              // unsets failbit if any
          } else {
              split_buf << *c;
            }
      }
    split_buf >> sgr_param;
    sgr_parts.push_back(sgr_param);
    return sgr_parts;
}

std::string ANSI_SGR2HTML::processSGR(SGRParts& sgr_parts/*non const!*/)
{
    std::string out;                                        // наверное, нет смысла резервировать размер. ОН будет меняться лишь 1-3 раза за функцию.

    if (sgr_parts.empty())
        return out;                                         //нечего парсить!
    unsigned int sgr_code = sgr_parts[0];

    switch (sgr_code) {
    case 0:         // Reset / Normal	all attributes off
        resetAll(out);
        break;

    case 1:         // Bold or increased intensity
        out.append("<b>");
        stack_intensity.push("</b>");
        break;
    case 3:         // Italic
        out.append("<i>");
        stack_italic.push("</i>");
        break;
    case 4:         // Underline
        out.append("<u>");
        stack_underline.push("</u>");
        break;
    case 9:         // Crossed-out
        out.append("<s>");
        stack_cross_out.push("</s>");
        break;
    case 22:        // Normal color or intensity
        resetAttribute(stack_intensity, out);
        break;
    case 23:        // Not italic, not Fraktur
        resetAttribute(stack_italic, out);
        break;
    case 24:        // Underline off
        resetAttribute(stack_underline, out);
        break;
    case 29:        // Not crossed out
        resetAttribute(stack_cross_out, out);
        break;
    case 39:        // Default foreground color
        resetAttribute(stack_fg_color, out);
        break;
    case 49:        // Default background color
        resetAttribute(stack_bg_color, out);
        break;
    case 38:        // Set foreground color
        if (5 == sgr_parts[1] && sgr_parts.size() >= 3) {   // 8-bit foreground color // 38:5:⟨n⟩
            out.append(R"(<font color=")");
            out.append(colors_256[sgr_parts[2]]);
            out.append(R"(">)");
            sgr_parts.pop_front();
            sgr_parts.pop_front();
            sgr_parts.pop_front();
            stack_fg_color.push("</font>");
            // 24-bit foreground color //38;2;⟨r⟩;⟨g⟩;⟨b⟩
        } else if (2 == sgr_parts[1] && sgr_parts.size() >= 5) {
            //TODO
            std::cerr << "24-bit color not supported" << std::endl;
            sgr_parts.pop_front();
            sgr_parts.pop_front();
            sgr_parts.pop_front();
            sgr_parts.pop_front();
            sgr_parts.pop_front();
        } else {
            return out;
        }
        break;
    case 48:        // Set background color
        if (5 == sgr_parts[1] && sgr_parts.size() >= 3) {   // 8-bit background color // 48:5:⟨n⟩
            out.append(R"(<span style="background-color:)");
            out.append(colors_256[sgr_parts[2]]);
            out.append(R"(">)");
            sgr_parts.pop_front();
            sgr_parts.pop_front();
            sgr_parts.pop_front();
            stack_bg_color.push("</span>");
            // 24-bit background color //48;2;⟨r⟩;⟨g⟩;⟨b⟩
        } else if (2 == sgr_parts[1] && sgr_parts.size() >= 5) {
            //TODO
            std::cerr << "24-bit color not supported" << std::endl;
            sgr_parts.pop_front();
            sgr_parts.pop_front();
            sgr_parts.pop_front();
            sgr_parts.pop_front();
            sgr_parts.pop_front();
        } else {
            return out;
        }
        break;

    default:        // SGR code ranges
        if (30 <= sgr_code && 37 >= sgr_code) {          // foreground color from table
            // TODO: возможно вернуть <font color> если будет две версии парсера и это ничего не сломает.
            out.append(R"(<font color=")");                    // TODO: как-то красивее конструировать строку. Можно использовать {fmt} или подождать С++20 с eel.is/c++draft/format. Пока сойдёт так.
            out.append(colors_basic[sgr_code]);
            out.append(R"(">)");
            stack_fg_color.push("</font>");
        } else if (40 <= sgr_code && 47 >= sgr_code) {          // background color from table
            out.append(R"(<span style="background-color:)");
            out.append(colors_basic[sgr_code]);
            out.append(R"(">)");
            stack_bg_color.push("</span>");
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

std::string ANSI_SGR2HTML::detectHTMLSymbol(char symbol)
{
    switch (symbol) {
    case '\0':                                              //и так бывает?
        return "";
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
    default:
        return std::string(&symbol, 1);
    }
}

void ANSI_SGR2HTML::resetAll(std::string& out)
{
    resetAttribute(stack_intensity, out);
    resetAttribute(stack_italic, out);
    resetAttribute(stack_underline, out);
    resetAttribute(stack_cross_out, out);
    resetAttribute(stack_fg_color, out);
    resetAttribute(stack_bg_color, out);
}

template<typename T, typename U>
void ANSI_SGR2HTML::resetAttribute(T attribute_stack, std::basic_string<U>& out)
{
    static_assert (std::is_same_v<T, std::stack<const U*> >, "T must be std::stack of const CharT*");
    while (!attribute_stack.empty()) {
        out.append(attribute_stack.top());
        attribute_stack.pop();
    }
}
