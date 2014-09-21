#include "file_reader.h"
#include "file_error.h"
#include "file_access_denied.h"
#include "../config/server_config.h"

#include <fstream>
#include <iostream>
#include <boost/make_shared.hpp>

file_reader::file_reader(const std::string& doc_root, const std::string& index_filename)
    : _doc_root(boost::filesystem::canonical(boost::filesystem::absolute(doc_root))),
      _index_filename(index_filename),
      _cache(server_config::instance().get_cache_period())
{
}

file::ptr file_reader::read(const std::string& src, bool do_reading)
{
    using namespace boost::filesystem;


    path src_path;
    try {
        src_path = canonical(absolute(_doc_root / path(src)));
    } catch (std::exception& e) {
        throw file_error("File not found");
    }

    try {
        if (is_directory(src_path)) {
            src_path /= path(_index_filename);
            src_path = canonical(src_path);
        }
    } catch (std::exception& e) {
        throw file_access_denied();
    }

//    if (!path_contains_file(_doc_root, src_path)) {
//        throw file_error("Not found");
//    }

    auto s_path = src_path.generic_string();
    auto res = _cache.get(s_path);
    if (res != nullptr) {
        return res;
    } else {
        std::cout << "cache doesn't have this entry\n";
        std::lock_guard<std::mutex> lk(_m);
        std::cout << "Mutex locked\n";

        if ((res = _cache.get(s_path)) == nullptr) {
            std::fstream fs;
            fs.open(s_path, std::ios::in | std::ios::binary | std::ios::ate);
            if (fs.is_open()) {
                int size = fs.tellg();
                fs.seekg(0, std::ios::beg);

                char* data = nullptr;
                if (do_reading) {
                    data = new char[size];
                    fs.read(data, size);
                }
                fs.close();

                auto out = file::make_file();
                out->load(data, size, file::guess_mime(src_path.extension().generic_string()), false, _cache.count_expires());
                _cache.add(s_path, out);

                std::cout << "Mutex unlocked (added to cache)\n";
                return out;
            } else {
                std::cout << "Mutex unlocked (not found)\n";
                throw file_error("File not found");
            }
        } else {
            std::cout << "Mutex unlocked (file is in cache)\n";
            return res;
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
    if (!file.has_filename())
        return false;
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
