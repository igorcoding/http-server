#include "file_error.h"

file_error::file_error()
    : runtime_error("Error loading file")
{
}
