#ifndef FILE_ERROR_H
#define FILE_ERROR_H

#include <stdexcept>

class file_error : public std::runtime_error
{
public:
    file_error(const std::string& msg);
};

#endif // FILE_ERROR_H
