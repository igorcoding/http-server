#include "file_not_in_doc_root_error.h"

file_not_in_doc_root_error::file_not_in_doc_root_error()
    : file_error("Requested file is not in document root")
{
}
