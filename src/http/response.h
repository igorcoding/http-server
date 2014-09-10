#ifndef RESPONSE_H
#define RESPONSE_H

#include "request.h"

#include <vector>
#include <string>
#include <cstring>

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

class response
{
public:
    response();
    ~response();

    void set_status(status_codes::status_code status_code) {
        _status_code = status_code;
    }

    void assign_data(const std::string& str);
    void assign_data(const char* data, size_t size);

    template<typename InputIterator> void assign_headers(InputIterator first, InputIterator last);
    void add_header(const header& h);

    size_t get_data_size() {
        return _data_size;
    }

    std::string build();

private:
    static std::string code_to_str(status_codes::status_code code);

private:
    static const protocol _protocol;
    status_codes::status_code _status_code;
    std::vector<header> _headers;
    char* _data;
    size_t _data_size;
};

#endif // RESPONSE_H
