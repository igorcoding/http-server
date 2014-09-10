#include "file_error.h"

file_error::file_error(const std::string& msg)
    : runtime_error("Error loading file: " + msg)
{
}
