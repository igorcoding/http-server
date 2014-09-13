#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>

struct protocol
{
    static const std::string HTTP_BEGIN;
    std::string v_major;
    std::string v_minor;

    static protocol parse(const std::string& str);
    std::string to_string() const;
};

#endif // PROTOCOL_H
