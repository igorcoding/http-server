#include "listener.h"

#include <signal.h>

listener::listener(int port, size_t workers_count)
    : _port(port),
      _workers_count(workers_count),
      _io_manager(new io_service_manager(_workers_count)),
      _signals(_io_manager->get_io_service()),
      _acceptor(_io_manager->get_io_service())
{
    _signals.add(SIGINT);
    _signals.add(SIGTERM);
#ifdef SIGQUIT
    _signals.add(SIGQUIT);
#endif

    init_signal_handlers();

    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
    _acceptor.open(endpoint.protocol());
    _acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    _acceptor.bind(endpoint);
    _acceptor.listen();

    accept_handler();
}

listener::~listener()
{
    delete _io_manager;
}

void listener::run()
{
    std::cout << "Listening :" << _port << std::endl;
    _io_manager->run();
}


void listener::accept_handler()
{
    _connection.reset(new connection(_io_manager->get_io_service()));
    _acceptor.async_accept(_connection->socket(),
        [this](boost::system::error_code ec) {
            if (!_acceptor.is_open()) {
                return;
            }

            if (!ec) {
                std::cout << "Got connection\n";
                _connection->run();
            }

            accept_handler();
        }
    );
}

void listener::init_signal_handlers()
{
    _signals.async_wait(
        [this](boost::system::error_code /*ec*/, int /*signo*/) {
            _acceptor.close();
            _io_manager->stop();
    });
}
