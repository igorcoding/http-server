#include "request_handler.h"
#include "../config/server_config.h"
#include "../fs/file_not_in_doc_root_error.h"
#include "../fs/file_error.h"


request_handler& request_handler::instance()
{
    static request_handler rh;
    return rh;
}

void request_handler::handle(request* req, response* resp)
{
    auto uri = req->get_uri();
    file f;
    try {
        _freader.read(uri.c_str(), &f);
        resp->assign_data(f.get_data(), f.get_size());
        resp->add_header(common_headers::content_type(f.get_mime()));
        resp->set_status(status_codes::OK);
    } catch (file_not_in_doc_root_error& e) {
        resp->set_status(status_codes::FORBIDDEN);
        std::string msg = "File \"" + uri + "\" is forbidden";
        resp->assign_data(msg.c_str(), msg.length());
        resp->add_header(common_headers::content_type(mime_types::text_plain));
    } catch (file_error& e) {
        resp->set_status(status_codes::NOT_FOUND);
        std::string msg = "File \"" + uri + "\" not found";
        resp->assign_data(msg.c_str(), msg.length());
        resp->add_header(common_headers::content_type(mime_types::text_plain));
    }
}

request_handler::request_handler()
    : _freader(server_config::instance().get_document_root())
{

}
