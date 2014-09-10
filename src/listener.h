#ifndef CONNECTION_H
#define CONNECTION_H

#include <boost/asio.hpp>

class listener
{
public:
    listener(const std::string& address, int port);
    ~listener();

    void run();

private:
    void init_signal_handlers();

private:
    int _port;
    boost::asio::io_service _io_service;
    boost::asio::signal_set _signals;
    boost::asio::ip::tcp::acceptor _acceptor;
};

#endif // CONNECTION_H
