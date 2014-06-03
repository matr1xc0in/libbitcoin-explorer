/**
 * Copyright (c) 2011-2014 sx developers (see AUTHORS)
 *
 * This file is part of sx.
 *
 * sx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#pragma warning(push)
//Suppressing msvc warnings from boost that are heard to deal with
//because boost/algorithm carelessly defines _SCL_SECURE_NO_WARNINGS
//without testing it first. 
#pragma warning(disable : 4996)
#include <iomanip>
#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <bitcoin/bitcoin.hpp>
#include <sx/utility/console.hpp>
#pragma warning(pop)

namespace sx {

const char* get_arg(const int argc, const char* argv[],
    const std::string& fallback, const int index)
{
    return argc > index ? argv[index] : fallback.c_str();
}

const char* get_arg_or_stream(const int argc, const char* argv[], 
    std::istream& cin, const int index, const bool trim)
{
    return argc > index ? argv[index] : read_stream(cin, trim).c_str();
}

size_t get_args(const int argc, const char* argv[], 
    std::vector<std::string>& arguments, const int index)
{
    for (int i = index; i < argc; ++i)
    {
        std::string argument(argv[i]);
        if (!sx::is_option(argument))
            arguments.push_back(argument);
    }

    return arguments.size();
}

const char* get_filename(const int argc, const char* argv[], const int index)
{
    return argc > index ? argv[index] : STDIN_PATH_SENTINEL;
}

bool get_option(const int argc, const char* argv[], const std::string& option)
{
    for (int i = 1; i < argc; i++)
    {
        if (is_option(argv[i], option))
            return true;
    }

    return false;
}

bool is_false(const std::string& text)
{
    std::string arg(text);

    // case conversion is dependent upon the thread locale
    boost::algorithm::to_lower(arg);

    // the concept of string false is hardwired here
    return arg == "false" || arg == "0" || arg == "no";
}

bool is_option(const std::string& text)
{
    // -f
    bool is_short = text.size() == 2 && text[0] == '-' && text[1] != '-';

    // --foobar
    bool is_long = !is_short && text.size() > 2 && text[0] == '-' &&
        text[1] == '-' && text[2] != '-';

    return is_short || is_long;
}

bool is_option(const std::string& text, const std::string& option)
{
    std::string arg(text);
    boost::algorithm::to_lower(arg);

    // -f
    bool is_short = arg.size() == 2 && arg[0] == '-' && arg[1] == option[0];

    // --foobar
    bool is_long = !is_short && option.size() > 1 && arg == "--" + option;

    return is_short || is_long;
}

bool is_true(const std::string& text)
{
    std::string arg(text);

    // case conversion is dependent upon the thread locale
    boost::algorithm::to_lower(arg);

    // the concept of string true is hardwired here
    return arg == "true" || arg == "1" || arg == "yes";
}

void join(std::vector<std::string>& words, std::string& sentence, 
    const char* delimiter)
{
    sentence = boost::join(words, delimiter);
}

void line_out(std::ostream& stream, const char* line, 
    const size_t offset, const char* inset)
{
    // safe string length 
    size_t length = std::string(inset).length();

    // overflow safe assurance that offset is always non-negative
    size_t padding = length > offset ? 0 : offset - length;

    // output the inset-offset-line to the specified stream
    stream << inset << std::string(padding, ' ') << line << std::endl;
}

void line_out(std::ostream& stream, std::string& line,
    const size_t offset, const char* inset)
{
    return line_out(stream, line.c_str(), offset, inset);
}

void line_out(std::ostream& stream, const std::vector<char*>& lines,
    const size_t offset, const char* inset)
{
    // we allow empty multi-line values in source data
    if (lines.size() < 1)
        return;

    // emit the first line as inset-offset-line
    line_out(stream, lines[0], offset, inset);

    // emit the remaining lines as offset-line
    std::for_each(++lines.begin(), lines.end(), 
        [&](const char* line){ line_out(stream, line, offset); });
}

std::string read_stream(std::istream& cin, bool trim)
{
    std::istreambuf_iterator<char> first(cin), last;
    std::string result(first, last);
    if (trim)
        boost::algorithm::trim(result);
    return result;
}

void sleep_ms(const uint32_t milliseconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void split(std::string& sentence, std::vector<std::string>& words)
{
    boost::split(words, sentence, boost::is_any_of("\n\t "));
}

void terminate_process_on_error(const std::error_code& error)
{
    if (!error)
        return;
    bc::log_fatal() << error.message();
    exit(main_failure);
}

bool validate_argument_range(const int actual,
    const std::vector<char*>& message, const int minimum, 
    const int maximum)
{
    bool valid = ((actual >= minimum) && (maximum == 0 || actual <= maximum));
    if (!valid)
        line_out(std::cerr, message);
    return valid;
}

} // sx