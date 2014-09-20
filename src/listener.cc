#include "listener.h"

#include <signal.h>

listener::listener(int port, size_t workers_count)
    : _port(port),
      _workers_count(workers_count),
      _io_manager(_workers_count),
      _signals(_io_manager.get_io_service()),
      _acceptor(_io_manager.get_io_service()),
      _request_handler()
{
    _signals.add(SIGINT);
    _signals.add(SIGTERM);
#ifdef SIGQUIT
    _signals.add(SIGQUIT);
#endif

    init_signal_handlers();

    boost::asio::ip::tcp::resolver resolver(_acceptor.get_io_service());
    boost::asio::ip::tcp::resolver::query query("0.0.0.0", std::to_string(port));
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
    _acceptor.open(endpoint.protocol());
    _acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    _acceptor.bind(endpoint);
    _acceptor.listen();

    exec_accept();
}

listener::~listener()
{
}

void listener::run()
{
    std::cout << "Listening :" << _port << std::endl;
    _io_manager.run();
}


void listener::exec_accept()
{
    _connection.reset(new connection(_io_manager.get_io_service(), _request_handler));
    _acceptor.async_accept(_connection->socket(),
        [this](boost::system::error_code ec) {
            if (!ec) {
                _connection->run();
            }

            exec_accept();
        }
    );
}

void listener::init_signal_handlers()
{
    _signals.async_wait(
        [this](boost::system::error_code /*ec*/, int /*signo*/) {
            _io_manager.stop();
            _connection.reset();
    });
}
