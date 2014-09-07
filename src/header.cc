#include "header.h"
#include "exceptions/malformed_components.h"
#include "util/misc.h"

#include <boost/algorithm/string.hpp>

const std::string header::NAME_VALUE_SEPARATOR = ":";

header header::parse(const std::string& line)
{
    std::vector<std::string> pair;
    misc::split(line, NAME_VALUE_SEPARATOR, pair);

    if (pair.size() != 2)
        throw malformed_header();

    auto name = pair[0];
    auto value = pair[1];
    boost::trim(name);
    boost::trim(value);

    header h = { name, value };
    return h;
}

std::string header::to_string() const
{
    return name + NAME_VALUE_SEPARATOR + " " + value;
}
