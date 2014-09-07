#ifndef HEADER_H
#define HEADER_H

#include <string>

struct header
{
    std::string name;
    std::string value;

    static header parse(const std::string& line);

private:
    static const std::string NAME_VALUE_SEPARATOR;
};

#endif // HEADER_H
