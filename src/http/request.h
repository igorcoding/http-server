#ifndef REQUEST_H
#define REQUEST_H

#include "methods.h"
#include "header.h"
#include "protocol.h"
#include "chunk.h"

#include <vector>
#include <string>
#include <cstring>
#include <memory>

#include <atomic>

class request
{
public:
    request();
    ~request();
    void parse(const std::string& raw_request);
    bool add_chunk(const char* data, size_t size);

    methods::method get_method() const;
    const std::string& get_uri() const;
    const std::string& get_query() const;
    const protocol& get_protocol() const;
    const std::vector<header>& get_headers() const;
    bool is_malformed() const;
    bool is_ready() const;

//    static std::atomic_int n;

private:
    static request& make_malformed(request& req);
    void normalize_uri(const std::string& str);
    std::string merge_middle(const std::vector<std::string>& components);

private:
    methods::method _method;
    std::string _uri;
    std::string _query;
    protocol _protocol;
    std::vector<header> _headers;
    bool _malformed;
    bool _ready;
    /// TODO: request body, do we really need it?

    std::vector<chunk::ptr> _chunks;
};


#endif // REQUEST_H
