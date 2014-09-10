#ifndef FILE_H
#define FILE_H

#include <cstdio>
#include <string>
#include <map>

namespace mime_types {
    enum mime_type
    {
        text_plain,
        text_html,
        text_css,
        text_xml,
        application_javascript,
        application_json,
        image_gif,
        image_jpeg,
        image_png
    };
}

class file
{
public:
    file();
    ~file();
    void load(const char* data, size_t size, mime_types::mime_type type);

    const char* get_data();
    size_t get_size();
    mime_types::mime_type get_mime();

    static mime_types::mime_type guess_mime(const std::string& extension);

private:
    char* _data;
    size_t _size;
    mime_types::mime_type _type;

    static std::map<std::string, mime_types::mime_type> _mimes;
};

#endif // FILE_H
