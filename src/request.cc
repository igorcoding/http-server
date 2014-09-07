#include "request.h"
#include "util/misc.h"
#include "exceptions/malformed_components.h"

request::request()
    : _malformed(false)
{ }

request request::parse(const std::string& raw_request)
{
    std::vector<std::string> lines;
    misc::split(raw_request, misc::crlf, lines);

    auto& first_line = lines[0];
    std::vector<std::string> first_line_components;
    misc::split(first_line, " ", first_line_components);


    request req;

    if (first_line_components.size() != FIRST_LINE_SIZE)
        return make_malformed(req);

    try {
        auto& last_line = *(std::end(lines) - 1);
        if (last_line != "") {
            throw malformed_request();
        }

        req._method = methods::assist::from_str(first_line_components[0]);
        req._uri = normalize_uri(first_line_components[1]);
        req._protocol = http_protocol::parse(first_line_components[2]);

        for (auto it = std::begin(lines) + 1; it != std::end(lines) - 1; ++it) {
            auto& line = *it;
            req._headers.push_back(header::parse(line));
        }

    } catch (malformed_request& e) {
        return make_malformed(req);
    }

    return req;
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

request& request::make_malformed(request& req)
{
    req._malformed = true;
    return req;
}

