#ifndef PPARSER_UTILS_H
#define PPARSER_UTILS_H

#include <iostream>
#include <cstring>
#include <vector>

class utils {
public:
    static void split(std::string const &str, const char* delim, std::vector<std::string> &out);
};


#endif //PPARSER_UTILS_H
