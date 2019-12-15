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
    typedef std::deque<unsigned int> SGRParts;
public:
    ANSI_SGR2HTML();
    ~ANSI_SGR2HTML();
    
    /**
     * @brief splitSGR parses string, with ANSI escape sequences to HTML string
     * @param raw_data string to parse
     * @return HTML string
     */
    std::string parse(std::string raw_data);

    ANSI_SGR2HTML(const ANSI_SGR2HTML &other) = delete;
    ANSI_SGR2HTML &operator=(const ANSI_SGR2HTML &other) = delete;
    
private:
    SGRParts splitSGR(const std::string& data);
    std::string processSGR(SGRParts& sgr_parts/*non const!*/);
    std::string detectHTMLSymbol(char symbol);
    void resetAll(std::string& out);
    void resetForegroundColor(std::string& out);
    void resetBackgroundColor(std::string& out);
    void resetIntensity(std::string& out);
    std::stack<const char*> stack_intensity;
    std::stack<const char*> stack_italic;
    std::stack<const char*> stack_fg_color;
    std::stack<const char*> stack_bg_color;
    std::unordered_map<unsigned char, const char*> colors_basic;
    std::unordered_map<unsigned char, const char*> colors_256;


};

#endif // ANSI_SGR2HTML_H
