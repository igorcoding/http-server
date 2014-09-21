#ifndef HEADER_H
#define HEADER_H

#include "../fs/file.h"

#include <string>
#include <iostream>


class header
{
public:
    header();
    header(const std::string& name, const std::string& value);

    static header parse(const std::string& line);
    std::string to_string() const;

    const std::string& get_name() const;
    void set_name(const std::string& name);
    const std::string& get_value() const;
    void set_value(const std::string& value);

private:
    std::string _name;
    std::string _value;
};

struct common_headers
{
    enum connection_state {
        keep_alve,
        close
    };

    static header content_type(mime_types::mime_type mime);
    static header content_length(int value);
    static header date(const std::string& date);
    static header server(const std::string& server);
    static header connection(connection_state state);
    static header expires(const std::string& expires_date);
};


#endif // HEADER_H
