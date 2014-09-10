#ifndef CONFIG_READ_ERROR_H
#define CONFIG_READ_ERROR_H

#include <stdexcept>

class config_read_error : public std::runtime_error
{
public:
    config_read_error(const std::string& reason);
};

#endif // CONFIG_READ_ERROR_H
