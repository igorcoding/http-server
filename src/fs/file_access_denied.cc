#include "file_access_denied.h"

file_access_denied::file_access_denied()
    : file_error("Requested file is not in document root")
{
}
