#ifndef MISC_H
#define MISC_H

#include <vector>
#include <string>

namespace misc
{

static const char crlf[] = {'\r', '\n'};

std::vector<std::string>& split(const std::string& s, const std::string& delim, std::vector<std::string>& elems);

}

#endif // MISC_H
