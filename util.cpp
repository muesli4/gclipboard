#include "util.hpp"

std::string && replace_special_whitespace_characters(std::string && s)
{
    char const * const whitespace_characters = "\n\t";

    std::size_t pos = s.find_first_of(whitespace_characters);

    while (pos != std::string::npos)
    {
        s.replace(pos, 1, 1, ' ');
        pos = s.find_first_of(whitespace_characters, pos);
    }

    return std::move(s);
}

