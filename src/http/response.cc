#include "response.h"
#include "../util/misc.h"

#include <sstream>

std::atomic_int response::n(0);

const protocol response::_protocol = { 1, 1 };

response::response()
    : _data(nullptr)
{

}

response::~response()
{
    if (_data != nullptr && _data->is_delete_required())
        delete _data;
}

void response::assign_data(file::ptr f)
{
    _data = f;
    add_header(common_headers::content_type(f->get_mime()));
    add_header(std::move(common_headers::content_length(f->get_size())));
}

void response::add_header(const header& h)
{
    _headers.push_back(h);
}

const std::string& response::build(bool send_data)
{
    std::stringstream ss;
    ss << _protocol.to_string() << " "
       << static_cast<int>(_status_code) << " "
       << code_to_str() << misc::crlf;
    for (auto& h : _headers) {
        ss << h.to_string() << misc::crlf;
    }


    ss << misc::crlf;

    if (send_data && !_data->is_empty()) {
        ss << std::string(get_data(), get_data_size());
    }

    _raw_response = ss.str();
    return _raw_response;
}

protocol response::get_protocol() const
{
    return _protocol;
}

status_codes::status_code response::get_status_code() const
{
    return _status_code;
}

std::string& response::get_status_line()
{
    ++n;
    std::stringstream ss;
    ss << _protocol.to_string() << " "
       << static_cast<int>(_status_code) << " "
       << code_to_str() << misc::crlf;
    _status_line = ss.str();
    return _status_line;
}

const std::vector<header>& response::get_headers() const
{
    return _headers;
}

const char* response::get_data() const
{
    return _data->get_data();
}

size_t response::get_data_size() const
{
    return _data->get_size();
}

time_t response::get_expires() const
{
    return static_cast<time_t>(_data->get_expires());
}

std::string response::code_to_str() const
{
    using namespace status_codes;
    switch (_status_code) {
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
