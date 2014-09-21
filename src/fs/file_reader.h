#ifndef FILE_READER_H
#define FILE_READER_H

#include "file.h"
#include "cache.h"

#include <boost/filesystem.hpp>
#include <string>
#include <mutex>

class file_reader
{
public:
    file_reader(const std::string& doc_root, const std::string& index_filename);
    file::ptr read(const std::string& src, bool do_reading = true);

private:
    bool path_contains_file(boost::filesystem::path dir, boost::filesystem::path file);

private:
    boost::filesystem::path _doc_root;
    std::string _index_filename;
    cache _cache;

    std::mutex _m;
};

#endif // FILE_READER_H
