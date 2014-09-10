#include "file_reader.h"
#include "file_error.h"
#include "file_not_in_doc_root_error.h"

#include <fstream>
#include <iostream>

file_reader::file_reader(const std::string& doc_root)
    : _doc_root(doc_root)
{
}

void file_reader::read(const char* src, file* out, bool do_reading)
{
    using namespace boost::filesystem;

    if (out == nullptr)
        return;

    path doc_root_path(_doc_root);
    doc_root_path = canonical(absolute(doc_root_path));


    path src_path(src);
    try {
        src_path = canonical(absolute(doc_root_path / src_path));
    } catch (std::exception& e) {
        throw file_error("File not found");
    }


    if (!path_contains_file(doc_root_path, src_path)) {
        throw file_not_in_doc_root_error();
    }

    std::fstream fs;
    fs.open(src_path.generic_string(), std::ios::in | std::ios::binary | std::ios::ate);
    if (fs.is_open()) {
        int size = fs.tellg();
        fs.seekg(0, std::ios::beg);

        char* data = nullptr;
        if (do_reading) {
            data = new char[size];
            fs.read(data, size);
        }

        fs.close();

        out->load(data, size, file::guess_mime(src_path.extension().generic_string()));
    } else {
        throw file_error("File not found");
    }
    fs.close();
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
