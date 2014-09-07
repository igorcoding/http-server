#ifndef RESPONSE_H
#define RESPONSE_H

#include "request.h"

#include <vector>
#include <string>

namespace status_codes
{
    enum status_code
    {
        OK = 200,

    };

}

class response
{
public:
    response();

    void set_status(status_codes::status_code status_code) {
        _status_code = status_code;
    }

    void append_data(const std::string& piece) {
        _data.append(piece);
    }

    template<typename InputIterator> void assign_headers(InputIterator first, InputIterator last);
    void add_header(const header& h);

    std::string build();

private:
    static std::string code_to_str(status_codes::status_code code);

private:
    static const http_protocol _protocol;
    status_codes::status_code _status_code;
    std::vector<header> _headers;
    std::string _data;
};

#endif // RESPONSE_H
