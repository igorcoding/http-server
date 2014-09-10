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
        image_png,
        application_x_shockwave_flash
    };
}

class file
{
public:
    file();
    ~file();
    void load(const char* data, size_t size, mime_types::mime_type type);
    void load(const std::string& s);

    const char* get_data() const;
    size_t get_size() const;
    mime_types::mime_type get_mime() const;

    static mime_types::mime_type guess_mime(const std::string& extension);

private:
    const char* _data;
    size_t _size;
    mime_types::mime_type _type;

    static std::map<std::string, mime_types::mime_type> _mimes;
};

#endif // FILE_H
