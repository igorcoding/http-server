#ifndef REQUEST_H
#define REQUEST_H

#include "methods.h"
#include "header.h"
#include "protocol.h"

#include <vector>
#include <string>
#include <cstring>
#include <memory>

class chunk
{
public:
    chunk(const char* buf, size_t size)
        : _data(nullptr),
          _size(size)
    {
        _data = new char[_size];
        memcpy(_data, buf, _size);
    }

    ~chunk()
    {
        delete[] _data;
    }

    const char* data()
    {
        return _data;
    }

    size_t size()
    {
        return _size;
    }

private:
    char* _data;
    size_t _size;
};

typedef std::shared_ptr<chunk> chunk_ptr;



class request
{
    static constexpr int FIRST_LINE_SIZE = 3;
public:
    request();
    ~request();
    void parse(const std::string& raw_request);
    bool add_chunk(const char* data, size_t size);

    methods::method get_method() const;
    const std::string& get_uri() const;
    const protocol& get_protocol() const;
    const std::vector<header>& get_headers() const;
    bool is_malformed() const;



private:
    static request& make_malformed(request& req);
    static std::string normalize_uri(const std::string& str);
    std::string merge_chunks() const;

private:
    methods::method _method;
    std::string _uri;
    protocol _protocol;
    std::vector<header> _headers;
    bool _malformed;
    /// TODO: request body, do we really need it?

    std::vector<chunk_ptr> _chunks;
};


#endif // REQUEST_H
