#include "methods.h"

namespace methods
{

std::map<std::string, method> assist::_method_str
            = mapper<std::string, method>("OPTIONS", OPTIONS)
                    ("GET", GET)
                    ("HEAD", HEAD)
                    ("POST", POST)
                    ("PUT", PUT)
                    ("PATCH", PATCH)
                    ("DELETE", DELETE)
                    ("TRACE", TRACE)
                    ("CONNECT", CONNECT);
}
