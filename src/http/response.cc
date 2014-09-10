#include "response.h"
#include "../util/misc.h"

#include <sstream>

const protocol response::_protocol = { 1, 1 };

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

void response::assign_data(const std::string& str)
{
    assign_data(str.c_str(), str.length());
}

void response::assign_data(const char* data, size_t size) {
    _data_size = size;
    delete[] _data;
    if (data != nullptr) {
        _data = new char[_data_size];
        memcpy(_data, data, _data_size);
    }

    add_header(std::move(common_headers::content_length(size)));
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
    for (auto& h : _headers) {
        ss << h.to_string() << misc::crlf;
    }


    ss << misc::crlf;

    if (_data != nullptr) {
        ss << _data;
    }

    return ss.str();
}

std::string response::code_to_str(status_codes::status_code code)
{
    using namespace status_codes;
    switch (code) {
    case OK:
        return "OK";
    case BAD_REQUEST:
        return "Bad Request";
    case FORBIDDEN:
        return "Access Denied";
    case NOT_FOUND:
        return "Not Found";
    case METHOD_NOT_ALLOWED:
        return "Method Not Allowed";
    default:
        break;
    }

    return "";
}
