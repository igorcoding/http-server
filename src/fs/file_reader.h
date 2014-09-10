#ifndef FILE_READER_H
#define FILE_READER_H

#include "file.h"

#include <boost/filesystem.hpp>
#include <string>

class file_reader
{
public:
    file_reader(const std::string& doc_root);
    void read(const char* src, file* out, bool do_reading = true);

private:
    bool path_contains_file(boost::filesystem::path dir, boost::filesystem::path file);

private:
    std::string _doc_root;
};

#endif // FILE_READER_H
