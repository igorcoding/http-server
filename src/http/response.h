#ifndef RESPONSE_H
#define RESPONSE_H

#include "header.h"
#include "protocol.h"
#include "../fs/file.h"

#include <vector>
#include <string>
#include <cstring>
#include <boost/asio.hpp>

namespace status_codes
{
    enum status_code
    {
        OK = 200,
        BAD_REQUEST = 400,
        FORBIDDEN = 403,
        NOT_FOUND = 404,
        METHOD_NOT_ALLOWED = 405

    };

}

namespace status_strings {


boost::asio::const_buffer to_buffer(status_codes::status_code code);

} // namespace status_strings

class response
{
public:
    response();
    ~response();

    void set_status(status_codes::status_code status_code) {
        _status_code = status_code;
    }

    void assign_data(file::ptr f);
    void add_header(const header& h);

//    std::string build();
    protocol get_protocol() const;
    status_codes::status_code get_status_code() const;
    const std::vector<header>& get_headers() const;
    const char* get_data() const;
    size_t get_data_size() const;
    time_t get_expires() const;
    std::string& get_status_line();

    std::string code_to_str() const;

private:
    static const protocol _protocol;
    status_codes::status_code _status_code;
    std::vector<header> _headers;
    file::ptr _data;

    std::string _status_line;
};

#endif // RESPONSE_H
