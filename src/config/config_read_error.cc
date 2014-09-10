#include "config_read_error.h"

config_read_error::config_read_error(const std::string& reason)
    : runtime_error("Error reading config file: " + reason)
{
}
