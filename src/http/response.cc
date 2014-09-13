#include "response.h"
#include "../util/misc.h"

#include <sstream>

namespace status_strings {

const std::string OK = "HTTP/1.1 200 OK\r\n";
const std::string BAD_REQUEST = "HTTP/1.1 400 Bad Request\r\n";
const std::string FORBIDDEN = "HTTP/1.1 403 Forbidden\r\n";
const std::string NOT_FOUND = "HTTP/1.1 404 Not Found\r\n";
const std::string METHOD_NOT_ALLOWED = "HTTP/1.1 405 Method Not Allowed\r\n";
const std::string INTERNAL_SERVER_ERROR = "HTTP/1.1 500 Internal Server Error\r\n";

boost::asio::const_buffer to_buffer(status_codes::status_code code)
{
    switch (code) {
    case status_codes::OK:
        return boost::asio::buffer(OK);
    case status_codes::BAD_REQUEST:
        return boost::asio::buffer(BAD_REQUEST);
    case status_codes::FORBIDDEN:
        return boost::asio::buffer(FORBIDDEN);
    case status_codes::NOT_FOUND:
        return boost::asio::buffer(NOT_FOUND);
    case status_codes::METHOD_NOT_ALLOWED:
        return boost::asio::buffer(METHOD_NOT_ALLOWED);
    default:
        return boost::asio::buffer(INTERNAL_SERVER_ERROR);
    }
}

} // namespace status_strings

const protocol response::_protocol = { "1", "1" };

response::response()
    : _data(nullptr)
{

}

response::~response()
{
}

void response::assign_data(file_ptr f)
{
    _data = f;
    add_header(common_headers::content_type(f->get_mime()));
    add_header(std::move(common_headers::content_length(f->get_size())));
}

void response::add_header(const header& h)
{
    _headers.push_back(h);
}

//std::string response::build()
//{
//    std::stringstream ss;
//    ss << _protocol.to_string() << " "
//       << static_cast<int>(_status_code) << " "
//       << code_to_str(_status_code) << misc::crlf;
//    for (auto& h : _headers) {
//        ss << h.to_string() << misc::crlf;
//    }


//    ss << misc::crlf;

//    if (!_data->is_empty()) {
//        ss << std::string(get_data(), get_data_size());
//    }

//    return ss.str();
//}

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
