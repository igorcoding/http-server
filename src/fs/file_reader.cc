#include "file_reader.h"
#include "file_error.h"

#include <fstream>
#include <iostream>

file_reader::file_reader(const std::string& doc_root)
    : _doc_root(doc_root)
{
}

void file_reader::read(const char* src, file* out)
{
    using namespace boost::filesystem;

    if (out == nullptr)
        return;

    path src_path(src);
    src_path = canonical(absolute(src_path));

    path doc_root_path(_doc_root);
    doc_root_path = canonical(absolute(doc_root_path));

    if (path_contains_file(doc_root_path, src_path)) {
        std::fstream fs;
        fs.open(src_path.generic_string(), std::ios::in | std::ios::binary | std::ios::ate);
        if (fs.is_open()) {
            int size = fs.tellg();
            char* data = new char[size];
            fs.seekg(0, std::ios::beg);
            fs.read(data, size);
            fs.close();

            out->load(data, size, file::guess_mime(src_path.extension().generic_string()));
            delete[] data;
        } else {
            throw file_error();
        }
        fs.close();
    }

    throw file_error();
}

bool file_reader::path_contains_file(boost::filesystem::path dir, boost::filesystem::path file)
{
    // If dir ends with "/" and isn't the root directory, then the final
    // component returned by iterators will include "." and will interfere
    // with the std::equal check below, so we strip it before proceeding.
    if (dir.filename() == ".")
    dir.remove_filename();
    // We're also not interested in the file's name.
    assert(file.has_filename());
    file.remove_filename();

    // If dir has more components than file, then file can't possibly
    // reside in dir.
    auto dir_len = std::distance(dir.begin(), dir.end());
    auto file_len = std::distance(file.begin(), file.end());
    if (dir_len > file_len)
        return false;

    // This stops checking when it reaches dir.end(), so it's OK if file
    // has more directory components afterward. They won't be checked.
    return std::equal(dir.begin(), dir.end(), file.begin());
}
