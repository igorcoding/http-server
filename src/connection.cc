#include "connection.h"
#include "util/misc.h"
#include "http/request_handler.h"

#include <boost/bind.hpp>
#include <cstdlib>

connection::connection(boost::asio::io_service& io_service, request_handler& req_handler)
    : _socket(io_service),
      _req(new request),
      _resp(new response),
      _request_handler(req_handler)
{
}

connection::~connection()
{
    delete _resp;
    delete _req;
}

void connection::run()
{
    exec_read();
}

connection::socket_t& connection::socket()
{
    return _socket;
}

void connection::exec_read()
{
    _socket.async_read_some(boost::asio::buffer(_buf),
                            boost::bind(&connection::read_handle, shared_from_this(),
                                        boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void connection::read_handle(boost::system::error_code e, size_t bytes)
{
    if (e) {
        return;
    }
    bool ready = _req->add_chunk(_buf.data(), bytes);
    if (ready) {
        _request_handler.handle(_req, _resp);
        boost::asio::async_write(_socket, to_asio_buffers(*_resp),
                                 boost::bind(&connection::write_handle, shared_from_this(),
                                             boost::asio::placeholders::error));
    } else {
        exec_read();
    }
}

void connection::write_handle(boost::system::error_code e)
{
    if (!e) {
        boost::system::error_code ec;
        _socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    }
}

std::vector<boost::asio::const_buffer> connection::to_asio_buffers(response& resp)
{
    std::vector<boost::asio::const_buffer> bufs;
    //"HTTP/1.1 200 OK\r\n"

//    bufs.push_back(status_strings::to_buffer(resp.get_status_code()));
//    bufs.push_back(boost::asio::buffer(resp.get_protocol().HTTP_BEGIN));
//    bufs.push_back(boost::asio::buffer(resp.get_protocol().v_major));
//    bufs.push_back(boost::asio::buffer("."));
//    bufs.push_back(boost::asio::buffer(resp.get_protocol().v_minor));
//    bufs.push_back(boost::asio::buffer(" "));
//    bufs.push_back(boost::asio::buffer(std::to_string(resp.get_status_code())));
//    bufs.push_back(boost::asio::buffer(" "));
//    bufs.push_back(boost::asio::buffer(resp.code_to_str()));
//    bufs.push_back(boost::asio::buffer(misc::crlf_arr));

    bufs.push_back(boost::asio::buffer(resp.get_status_line()));


    for (auto& h : resp.get_headers()) {
        bufs.push_back(boost::asio::buffer(h.get_name()));
        bufs.push_back(boost::asio::buffer(misc::namevalue_sep));
        bufs.push_back(boost::asio::buffer(h.get_value()));
        bufs.push_back(boost::asio::buffer(misc::crlf_arr));
    }
    bufs.push_back(boost::asio::buffer(misc::crlf_arr));
    if (resp.get_data() != nullptr) {
        bufs.push_back(boost::asio::buffer(resp.get_data(), resp.get_data_size()));
    }
    return bufs;
}
