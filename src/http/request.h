#ifndef REQUEST_H
#define REQUEST_H

#include "methods.h"
#include "header.h"
#include "protocol.h"

#include <vector>
#include <string>

class request
{
    static constexpr int FIRST_LINE_SIZE = 3;
public:
    request();
    void parse(const std::string& raw_request);

    methods::method get_method() const;
    const std::string& get_uri() const;
    const protocol& get_protocol() const;
    const std::vector<header>& get_headers() const;
    bool is_malformed() const;
    const std::string& get_raw() const;

private:
    static request& make_malformed(request& req);
    static std::string normalize_uri(const std::string& str);

private:
    methods::method _method;
    std::string _uri;
    protocol _protocol;
    std::vector<header> _headers;
    bool _malformed;

    std::string _raw;
};


#endif // REQUEST_H
