#ifndef RESPONSE_H
#define RESPONSE_H

#include "request.h"

#include <string>

class response
{
public:
    response();

    std::string build();

private:
    static constexpr http_protocol _protocol = {1, 0};
    int _status_code;
    std::string _status_str;
    std::string _data;
};

#endif // RESPONSE_H
