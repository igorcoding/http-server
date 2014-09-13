#ifndef LISTENER_H
#define LISTENER_H

#include "connection.h"
#include "io_service_manager.h"

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <thread>

class listener
{
    typedef boost::asio::ip::tcp::socket sock_t;
    typedef boost::shared_ptr<sock_t> sock_ptr;
public:
    listener(int port, size_t workers_count);
    ~listener();

    void run();

private:
    void accept_handler();
    void init_signal_handlers();

private:
    int _port;
    size_t _workers_count;
    io_service_manager* _io_manager;
    boost::asio::signal_set _signals;
    boost::asio::ip::tcp::acceptor _acceptor;
    connection_ptr _connection;
};

#endif // LISTENER_H
