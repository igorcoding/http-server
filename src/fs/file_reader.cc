#include "file_reader.h"
#include "file_error.h"
#include "file_access_denied.h"
#include "../http/chunk.h"

#include <fstream>
#include <iostream>

file_reader::file_reader(const std::string& doc_root, const std::string& index_filename)
    : _doc_root(doc_root),
      _index_filename(index_filename)
{
}

void file_reader::read(const std::string& src, file_ptr out, bool do_reading)
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

    try {
        if (is_directory(src_path)) {
            src_path /= path(_index_filename);
            src_path = canonical(absolute(src_path));
        }
    } catch (std::exception& e) {
        throw file_access_denied();
    }

//    if (!path_contains_file(doc_root_path, src_path)) {
//        throw file_error("Not found");
//    }

    std::fstream fs;
    if (!do_reading) {
        fs.open(src_path.generic_string(), std::ios::in | std::ios::binary | std::ios::ate);
        if (fs.is_open()) {
            int size = fs.tellg();
            fs.close();

            out->load(nullptr, size, file::guess_mime(src_path.extension().generic_string()));
        } else {
            throw file_error("File not found");
        }
    } else {
        fs.open(src_path.generic_string(), std::ios::in | std::ios::binary);
        if (fs.is_open()) {
            std::vector<chunk::ptr> chunks;
            char buf[512];
            while (fs.read(buf, sizeof(buf)).gcount() > 0)
                chunks.push_back(boost::make_shared<chunk>(buf, fs.gcount()));
            auto merged = chunk::merge_chunks(chunks);

            fs.close();

            out->load(merged->data(), merged->size(), file::guess_mime(src_path.extension().generic_string()));
        } else {
            throw file_error("File not found");
        }
    }
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
