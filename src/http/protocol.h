#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>

struct protocol
{
    int v_major;
    int v_minor;

    static protocol parse(const std::string& str);
    std::string to_string() const;

private:
    static const std::string HTTP_BEGIN;
};

#endif // PROTOCOL_H
