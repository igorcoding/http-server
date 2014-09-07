#ifndef REQUEST_H
#define REQUEST_H

#include "methods.h"
#include "header.h"
#include "http_protocol.h"

#include <vector>
#include <string>

class request
{
    static constexpr int FIRST_LINE_SIZE = 3;
public:
    static request parse(const std::string& raw_request);

    bool is_malformed() {
        return _malformed;
    }

private:
    request();
    static request& make_malformed(request& req);
    static std::string normalize_uri(const std::string& str);

private:
    methods::method _method;
    std::string _uri;
    http_protocol _protocol;
    std::vector<header> _headers;
    bool _malformed;
};


#endif // REQUEST_H
