#include "utils.h"

void utils::split(std::string const &str, const char* delim, std::vector<std::string> &out){
    char *token = strtok(const_cast<char*>(str.c_str()), delim);
    while (token != nullptr){
        out.emplace_back(token);
        token = strtok(nullptr, delim);
    }
}
