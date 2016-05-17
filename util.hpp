#ifndef GCLIPBOARD_UTIL_HPP
#define GCLIPBOARD_UTIL_HPP

#include <string>

std::string replace_special_whitespace_characters(std::string s);

std::string shorten_string(std::string s, std::size_t max_len);

#endif

