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

#ifndef ANSI_SGR2HTML_H
#define ANSI_SGR2HTML_H

#include <deque>
#include <stack>
#include <string>
#include <unordered_map>

/**
 * @todo write docs
 */
class ANSI_SGR2HTML
{
    typedef std::deque<unsigned char> SGRParts;
public:
    ANSI_SGR2HTML();
    ~ANSI_SGR2HTML();
    
    /**
     * @brief simpleParse parses string, with ANSI escape sequences to HTML string
     * @param raw_data string to parse
     * @return HTML string
     */
    std::string simpleParse(std::string raw_data);

    ANSI_SGR2HTML(const ANSI_SGR2HTML &other) = delete;
    ANSI_SGR2HTML &operator=(const ANSI_SGR2HTML &other) = delete;
    
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
    //TODO: make color tables static constexpr to reduce footprint of object creation.
    static const std::unordered_map<unsigned char, const char*> colors_basic_; //can't constexpr maps so other trick used
};

#endif // ANSI_SGR2HTML_H
