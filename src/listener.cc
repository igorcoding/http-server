#include "listener.h"

#include <signal.h>

listener::listener(const std::string& address, int port)
    : _port(port),
      _io_service(),
      _signals(_io_service),
      _acceptor(_io_service)
{
    _signals.add(SIGINT);
    _signals.add(SIGTERM);
#if defined(SIGQUIT)
    _signals.add(SIGQUIT);
#endif

    init_signal_handlers();

    boost::asio::ip::tcp::resolver resolver(_io_service);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({address, std::to_string(port)});
    _acceptor.open(endpoint.protocol());
    _acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    _acceptor.bind(endpoint);
    _acceptor.listen();
}

listener::~listener()
{

}

void listener::run()
{
    std::cout << "Listening :" << _port << std::endl;
    _io_service.run();
}

void listener::init_signal_handlers()
{
    _signals.async_wait(
        [this](boost::system::error_code /*ec*/, int /*signo*/) {
            _acceptor.close();
            /// TODO: close everything
        });
}
