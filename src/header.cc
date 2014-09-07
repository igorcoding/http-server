#include "header.h"
#include "util/misc.h"
#include "exceptions/malformed_components.h"
#include <boost/algorithm/string.hpp>

const std::string header::NAME_VALUE_SEPARATOR = ":";

header header::parse(const std::string& line)
{
    std::vector<std::string> pair;
    misc::split(line, NAME_VALUE_SEPARATOR, pair);

    if (pair.size() != 2)
        throw malformed_header();

    header h = { pair[0], pair[1] };
    boost::trim(h.name);
    boost::trim(h.value);
}
