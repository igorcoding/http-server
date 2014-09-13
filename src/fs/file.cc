#include "file.h"
#include "../util/mapper.h"

#include <cstring>

std::map<std::string, mime_types::mime_type> file::_mimes
                    = mapper<std::string, mime_types::mime_type>(".html", mime_types::text_html)
                                                                (".htm", mime_types::text_html)
                                                                (".css", mime_types::text_css)
                                                                (".js", mime_types::application_javascript)
                                                                (".xml", mime_types::text_xml)
                                                                (".json", mime_types::application_json)
                                                                (".jpg", mime_types::image_jpeg)
                                                                (".jpeg", mime_types::image_jpeg)
                                                                (".gif", mime_types::image_gif)
                                                                (".png", mime_types::image_png)
                                                                (".swf", mime_types::application_x_shockwave_flash);

void file::load(const char* data, size_t size, mime_types::mime_type type)
{
    delete[] _data;
    _size = size;
    _type = type;
    if (data != nullptr) {
        _data = new char[size];
        memcpy(_data, data, size);
    }
}

void file::load(const std::string& s)
{
    load(s.c_str(), s.length(), mime_types::text_plain);
}

file::file()
    : _data(nullptr),
      _size(0),
      _type(mime_types::text_plain)
{ }

file::~file()
{
    delete[] _data;
    _size = 0;
    _type = mime_types::text_plain;
}

const char* file::get_data() const
{
    return _data;
}

size_t file::get_size() const
{
    return _size;
}

mime_types::mime_type file::get_mime() const
{
    return _type;
}

bool file::is_empty() const
{
    return _data == nullptr;
}

mime_types::mime_type file::guess_mime(const std::string& extension)
{
    auto m = _mimes.find(extension);
    if (m == _mimes.end()) {
        return mime_types::text_plain;
    } else {
        return m->second;
    }
}
