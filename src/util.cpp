#include "util.hpp"

#include <iostream>

std::string replace_special_whitespace_characters(std::string s)
{
    char const * const whitespace_characters = "\n\t";

    std::size_t pos = s.find_first_of(whitespace_characters);

    while (pos != std::string::npos)
    {
        s.replace(pos, 1, 1, ' ');
        pos = s.find_first_of(whitespace_characters, pos);
    }

    return s;
}

std::string shorten_string(std::string s, std::size_t max_len)
{
    if (s.size() > max_len)
    {

        std::cout << '\'' << s << '\'' << ' ' << max_len << std::endl;
        std::cout.flush();
        std::cout << '\'' << s.substr(0, max_len) << '\'' << std::endl;
        std::cout.flush();
        return s.substr(0, max_len).replace(max_len, 1, "...");
    }
    else
    {
        return s;
    }
}

