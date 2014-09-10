#include "request_handler.h"
#include "../config/server_config.h"
#include "../fs/file_not_in_doc_root_error.h"
#include "../fs/file_error.h"

#include <ctime>
#include <sstream>


request_handler& request_handler::instance()
{
    static request_handler rh;
    return rh;
}

void request_handler::handle(request* req, response* resp)
{
    handle_internal(req, resp);
    resp->add_header(common_headers::date(get_current_time()));
    resp->add_header(common_headers::connection(common_headers::connection_state::close));
}

void request_handler::handle_internal(request* req, response* resp)
{
    if (req->is_malformed()) {
        make_bad_request(resp);
        return;
    }

    if (!filter_request(req)) {
        resp->assign_data("Method not allowed");
        resp->set_status(status_codes::METHOD_NOT_ALLOWED);
        resp->add_header(common_headers::content_type(mime_types::text_plain));
        return;
    }

    std::string uri;
    bool decode_res = url_decode(req->get_uri(), uri);
    if (!decode_res) {
        make_bad_request(resp);
        return;
    }
    if (uri == "/") {
        uri = "/" + server_config::instance().get_index_filename();
    }
    auto m = req->get_method();
    file f;
    try {
        _freader.read(uri.c_str(), &f, m != methods::HEAD);
        resp->assign_data(f.get_data(), f.get_size());
        resp->add_header(common_headers::content_type(f.get_mime()));
        resp->set_status(status_codes::OK);
    } catch (file_not_in_doc_root_error& e) {
        resp->set_status(status_codes::FORBIDDEN);
        resp->assign_data("File \"" + uri + "\" is forbidden");
        resp->add_header(common_headers::content_type(mime_types::text_plain));
    } catch (file_error& e) {
        resp->set_status(status_codes::NOT_FOUND);
        resp->assign_data("File \"" + uri + "\" not found");
        resp->add_header(common_headers::content_type(mime_types::text_plain));
    }
}

std::string request_handler::get_current_time()
{
    const size_t buf_size = 32;
    char buffer [buf_size];

    time_t rawtime;
    struct tm* timeinfo;

    time (&rawtime);
    timeinfo = gmtime (&rawtime);

    int size = strftime (buffer, buf_size, "%a, %d %b %Y %T GMT", timeinfo);
    return std::string(buffer, size);
}

bool request_handler::filter_request(request* req)
{
    auto m = req->get_method();
    if (m != methods::GET && m != methods::HEAD) {
        return false;
    }
    return true;
}

bool request_handler::url_decode(const std::string& in, std::string& out)
{
    out.clear();
    out.reserve(in.size());
    for (std::size_t i = 0; i < in.size(); ++i) {
        if (in[i] == '%') {
            if (i + 3 <= in.size()) {
                int value = 0;
                std::istringstream is(in.substr(i + 1, 2));
                if (is >> std::hex >> value) {
                  out += static_cast<char>(value);
                  i += 2;
                }
                else {
                  return false;
                }
            } else {
                return false;
            }
        }
        else if (in[i] == '+') {
            out += ' ';
        } else {
          out += in[i];
        }
    }
    return true;
}

void request_handler::make_bad_request(response* resp)
{
    resp->assign_data("Bad request");
    resp->add_header(common_headers::content_type(mime_types::text_plain));
    resp->set_status(status_codes::BAD_REQUEST);
}

request_handler::request_handler()
    : _freader(server_config::instance().get_document_root())
{

}
