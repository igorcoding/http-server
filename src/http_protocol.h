#ifndef HTTP_PROTOCOL_H
#define HTTP_PROTOCOL_H

#include <string>

struct http_protocol
{
    int v_major;
    int v_minor;

    static http_protocol parse(const std::string& str);
    std::string to_string() const;

private:
    static const std::string HTTP_BEGIN;
};

#endif // HTTP_PROTOCOL_H
