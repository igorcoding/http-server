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



    try {
        auto& last_line = *(std::end(lines) - 1);
        if (last_line != "") {
            throw malformed_request();
        }

        _method = methods::assist::from_str(first_line_components[0]);
        normalize_uri(merge_middle(first_line_components));
        _protocol = protocol::parse(first_line_components.back());

        for (auto it = lines.begin() + 1; it != lines.end() - 1; ++it) {
            auto& line = *it;
            _headers.push_back(header::parse(line));
        }

    } catch (malformed_request& e) {
        make_malformed(*this);
    }
}

bool request::add_chunk(const char* data, size_t size)
{
    _chunks.push_back(boost::make_shared<chunk>(data, size));
    auto last = _chunks.back()->data();
    // TODO
    auto finished = strstr(last, misc::double_crlf);
    if (finished != nullptr) {
        auto merged = chunk::merge_chunks(_chunks);
        parse(std::string(merged->data(), merged->size()));
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

const std::string&request::get_query() const
{
    return _query;
}

const protocol& request::get_protocol() const
{
    return _protocol;
}

const std::vector<header>& request::get_headers() const
{
    return _headers;
}

void request::normalize_uri(const std::string& uri)
{
    if (uri == "") {
        throw malformed_uri();
    }

    size_t query_pos = uri.find('?');
    if (query_pos != std::string::npos) {
        _uri = uri.substr(0, query_pos);
        _query = uri.substr(query_pos);
    } else {
        _uri = uri;
    }
}

std::string request::merge_middle(const std::vector<std::string>& components)
{
    std::string s;
    size_t size = components.size() - 1;
    for (size_t i = 1; i < size - 1; ++i) {
        s += components[i] + " ";
    }
    s += components[size - 1];
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

