#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

void ltrim(std::string &s);
void rtrim(std::string &s);
void trim(std::string &s);
std::string to_string(int number);
std::vector<std::string> split(const std::string& str, char delimiter);

#endif