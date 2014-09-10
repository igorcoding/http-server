#include "header.h"
#include "exceptions/malformed_components.h"
#include "../util/misc.h"

#include <boost/algorithm/string.hpp>

header::header()
    : _name(""),
      _value("")
{ }

header::header(const std::string& name, const std::string& value)
    : _name(name),
      _value(value)
{ }

header header::parse(const std::string& line)
{
    auto colon = line.find(':');
    if (colon == std::string::npos)
        throw malformed_header();

    auto name = line.substr(0, colon);
    size_t value_index = colon + 1;
    if (value_index >= line.length())
        throw malformed_header();
    auto value = line.substr(colon + 1, line.length() - (colon+1));

    boost::trim(name);
    boost::trim(value);
    return header(name, value);
}

std::string header::to_string() const
{
    return _name + ": " + _value;
}

const std::string& header::get_name() const
{
    return _name;
}

void header::set_name(const std::string& name)
{
    _name = name;
}

const std::string& header::get_value() const
{
    return _value;
}

void header::set_value(const std::string& value)
{
    _value = value;
}





header common_headers::content_type(mime_types::mime_type mime)
{
    using namespace mime_types;

    std::string m = "";
    switch(mime) {
    case text_plain:
        m = "text/plain";
        break;
    case text_html:
        m = "text/html";
        break;
    case text_css:
        m = "text/css";
        break;
    case text_xml:
        m = "text/xml";
        break;
    case application_javascript:
        m = "application/javascript";
        break;
    case application_json:
        m = "application/json";
    case image_jpeg:
        m = "image/jpeg";
        break;
    case image_png:
        m = "image/png";
        break;
    case image_gif:
        m = "image/gif";
        break;
    case application_x_shockwave_flash:
        m = "application/x-shockwave-flash";
    default:
        m = "text/plain";
        break;
    }
    return header("Content-Type", m);
}

header common_headers::content_length(int value)
{
    return header("Content-Length", std::to_string(value));
}

header common_headers::date(const std::string& date)
{
    return header("Date", date);
}

header common_headers::server(const std::string& server)
{
    return header("Server", server);
}

header common_headers::connection(common_headers::connection_state state)
{
    std::string s = "";
    switch (state) {
    case keep_alve:
        s = "keep-alive";
        break;
    case connection_state::close:
        s = "close";
        break;
    }
    return header("Connection", s);
}
