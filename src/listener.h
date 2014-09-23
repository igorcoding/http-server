#ifndef LISTENER_H
#define LISTENER_H

#include "connection.h"
#include "io_service_manager.h"
#include "http/request_handler.h"

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <thread>
#include <atomic>

class listener
{
    typedef boost::asio::ip::tcp::socket sock_t;
    typedef boost::shared_ptr<sock_t> sock_ptr;
public:
    listener(int port, size_t workers_count);
    ~listener();

    void run();


private:
    void prefork(int workers_count);
    void exec_accept();
    void init_signal_handlers();

private:
    int _port;
    size_t _workers_count;
    boost::asio::io_service _io_service;
    boost::asio::io_service::work _work;
    boost::asio::signal_set _signals;
    boost::asio::ip::tcp::acceptor _acceptor;
    connection_ptr _connection;
    request_handler _request_handler;

    std::vector<pid_t> _forks;
};

#endif // LISTENER_H
