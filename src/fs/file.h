#ifndef FILE_H
#define FILE_H

#include <cstdio>
#include <string>
#include <map>
#include <boost/shared_ptr.hpp>

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
    void load(char* data, size_t size, mime_types::mime_type type, bool reqire_delete = true);
    void load(const std::string& s);

    char* get_data() const;
    size_t get_size() const;
    mime_types::mime_type get_mime() const;
    bool is_empty() const;
    bool is_delete_required() const;

    static mime_types::mime_type guess_mime(const std::string& extension);

private:
    char* _data;
    size_t _size;
    std::string _text_data;
    mime_types::mime_type _type;
    bool _require_delete;

    static std::map<std::string, mime_types::mime_type> _mimes;
};

typedef boost::shared_ptr<file> file_ptr;

#endif // FILE_H
