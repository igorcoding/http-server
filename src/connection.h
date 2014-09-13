#ifndef CONNECTION_H
#define CONNECTION_H

#include "http/request.h"
#include "http/response.h"

#include <memory>
#include <array>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

class connection : public boost::enable_shared_from_this<connection>,
                   private boost::noncopyable
{
    typedef boost::asio::ip::tcp::socket socket_t;
public:
    explicit connection(boost::asio::io_service& io_service);
    ~connection();

    void run();
    socket_t& socket();

private:
    void exec_read();
    void read_handle(boost::system::error_code e, size_t bytes);
    void write_handle(boost::system::error_code e, size_t bytes);
    static std::vector<boost::asio::const_buffer> to_asio_buffers(const response* resp);

private:
    socket_t _socket;
    std::array<char, 1024> _buf;
    request* _req;
    response* _resp;
};

typedef boost::shared_ptr<connection> connection_ptr;

#endif // CONNECTION_H
