#include "misc.h"

#include <boost/algorithm/string.hpp>

namespace misc {

std::vector<std::string>& split(const std::string& s, const std::string& delims, std::vector<std::string>& elems)
{
    boost::split(elems, s, boost::is_any_of(delims), boost::token_compress_on);
    return elems;
}

}
