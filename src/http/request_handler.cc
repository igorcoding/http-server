#include "request_handler.h"
#include "../config/server_config.h"
#include "../fs/file_access_denied.h"
#include "../fs/file_error.h"

#include <ctime>
#include <sstream>
#include <thread>

void request_handler::handle(request* req, response* resp)
{
    handle_internal(req, resp);
    resp->add_header(common_headers::date(get_current_time()));
    resp->add_header(common_headers::connection(common_headers::connection_state::close));
    resp->add_header(common_headers::server("igorcoding's Http Server"));
}

void request_handler::handle_internal(request* req, response* resp)
{
//    std::cout << std::this_thread::get_id() << "  " << req->get_uri() << std::endl;
    if (req->is_malformed()) {
        make_bad_request(resp);
        return;
    }

    if (!filter_request(req)) {
        auto f = boost::make_shared<file>();
        f->load("Method not allowed mothefuckers");
        resp->set_status(status_codes::METHOD_NOT_ALLOWED);
        resp->assign_data(f);
        return;
    }

    std::string uri;
    bool decode_res = url_decode(req->get_uri(), uri);
    if (!decode_res) {
        make_bad_request(resp);
        return;
    }

    auto m = req->get_method();

    try {
        file_ptr f = _freader.read(uri, m != methods::HEAD);
        resp->assign_data(f);
        resp->set_status(status_codes::OK);
    } catch (file_access_denied& e) {
        resp->set_status(status_codes::FORBIDDEN);
        auto f = boost::make_shared<file>();
        f->load("File \"" + uri + "\" is forbidden");
        resp->assign_data(f);
    } catch (file_error& e) {
        resp->set_status(status_codes::NOT_FOUND);
        auto f = boost::make_shared<file>();
        f->load("File \"" + uri + "\" not found");
        resp->assign_data(f);
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
    auto f = boost::make_shared<file>();
    f->load("Bad request");
    resp->assign_data(f);
    resp->add_header(common_headers::content_type(mime_types::text_plain));
    resp->set_status(status_codes::BAD_REQUEST);
}

request_handler::request_handler()
    : _freader(server_config::instance().get_document_root(), server_config::instance().get_index_filename())
{

}
