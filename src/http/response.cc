#include "response.h"
#include "../util/misc.h"

#include <sstream>

const http_protocol response::_protocol = { 1, 0 };

response::response()
    : _data(nullptr),
      _data_size(0)
{
}

response::~response()
{
    delete[] _data;
    _data_size = 0;
}

template <typename InputIterator>
void response::assign_headers(InputIterator first, InputIterator last)
{
    _headers.assign(first, last);
}

void response::add_header(const header& h)
{
    _headers.push_back(h);
}

std::string response::build()
{
    std::stringstream ss;
    ss << _protocol.to_string() << " "
       << static_cast<int>(_status_code) << " "
       << code_to_str(_status_code) << misc::crlf;
//    for (auto& h : _headers) {
//        ss << h.to_string() << misc::crlf;
//    }


    ss << misc::crlf;

    ss << _data;

    return ss.str();
}

std::string response::code_to_str(status_codes::status_code code)
{
    switch (code) {
    case status_codes::OK:
        return "OK";
    default:
        break;
    }

    return "";
}
