#ifndef METHODS_H
#define METHODS_H

#include "util/mapper.h"
#include "exceptions/malformed_components.h"

#include <map>
#include <string>

namespace methods
{
    enum method
    {
        OPTIONS,
        GET,
        HEAD,
        POST,
        PUT,
        PATCH,
        DELETE,
        TRACE,
        CONNECT
    };

    struct assist
    {
    public:
        static method from_str(std::string m) {
            auto i = _method_str.find(m);
            if (i == std::end(_method_str)) {
                throw unknown_method();
            } else {
                return i->second;
            }
        }

    private:
        static std::map<std::string, method> _method_str;
    };
}

#endif // METHODS_H
