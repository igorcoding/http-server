#ifndef FILE_ACCESS_DENIED_H
#define FILE_ACCESS_DENIED_H

#include "file_error.h"

class file_access_denied : public file_error
{
public:
    file_access_denied();
};

#endif // FILE_ACCESS_DENIED_H
