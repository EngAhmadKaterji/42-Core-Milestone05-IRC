#include "Utils.hpp"
#include <sstream>
#include <string>

void ltrim(std::string &s) {
    size_t start = 0;
    while (start < s.size() && std::isspace(s[start])) {
        ++start;
    }
    s = s.substr(start);
}
void rtrim(std::string &s) {
    size_t end = s.size();
    while (end > 0 && std::isspace(s[end - 1])) {
        --end;
    }
    s = s.substr(0, end);
}
void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}
std::string to_string(int number){
    std::ostringstream oss;
    oss << number;
    return oss.str();
}
std::vector<std::string> split(const std::string& str, char delimiter){
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while(std::getline(ss, token, delimiter)){
        tokens.push_back(token);
    }
    return tokens;
}