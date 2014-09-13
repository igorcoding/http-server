#include "request.h"
#include "../util/misc.h"
#include "exceptions/malformed_components.h"

request::request()
    : _malformed(false)
{ }

request::~request()
{
}

void request::parse(const std::string& raw_request)
{
    std::vector<std::string> lines;
    std::string splitter(misc::crlf);
    misc::split(raw_request, splitter, lines);

    auto& first_line = lines[0];
    std::vector<std::string> first_line_components;
    misc::split(first_line, " ", first_line_components);


    if (first_line_components.size() != FIRST_LINE_SIZE)
        make_malformed(*this);

    try {
        auto& last_line = *(std::end(lines) - 1);
        if (last_line != "") {
            throw malformed_request();
        }

        _method = methods::assist::from_str(first_line_components[0]);
        _uri = normalize_uri(first_line_components[1]);
        _protocol = protocol::parse(first_line_components[2]);

        for (auto it = std::begin(lines) + 1; it != std::end(lines) - 1; ++it) {
            auto& line = *it;
            _headers.push_back(header::parse(line));
        }

    } catch (malformed_request& e) {
        make_malformed(*this);
    }
}

bool request::add_chunk(const char* data, size_t size)
{
    _chunks.push_back(std::make_shared<chunk>(data, size));
    auto last = _chunks.back()->data();
    // TODO
    if (memcmp(last + size - 4, misc::double_crlf, 4) == 0) {
        auto merged = merge_chunks();
        parse(merged);
        return true;
    }
    return false;
}

methods::method request::get_method() const
{
    return _method;
}

const std::string& request::get_uri() const
{
    return _uri;
}

const protocol& request::get_protocol() const
{
    return _protocol;
}

const std::vector<header>& request::get_headers() const
{
    return _headers;
}

std::string request::normalize_uri(const std::string& uri)
{
    std::string normalized = uri;
    // TODO

    if (normalized == "") {
        throw malformed_uri();
    }
    return normalized;
}

std::string request::merge_chunks() const
{
    size_t total_size = 0;
    for (auto& c : _chunks) {
        total_size += c->size();
    }

    char* merged = new char[total_size];
    size_t offset = 0;
    for (auto c : _chunks) {
        memcpy(merged + offset, c->data(), c->size());
        offset += c->size();
    }

    auto s = std::string(merged, total_size);
    delete[] merged;
    merged = nullptr;
    return s;
}

request& request::make_malformed(request& req)
{
    req._malformed = true;
    return req;
}

bool request::is_malformed() const {
    return _malformed;
}

