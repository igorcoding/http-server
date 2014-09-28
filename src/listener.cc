#include "listener.h"

#include <signal.h>

listener::listener(int port, size_t workers_count)
    : _port(port),
      _workers_count(workers_count),
      _io_service(),
      _work(_io_service),
      _signals(_io_service),
      _acceptor(_io_service),
      _request_handler(),
      _forks()
{
    init_signal_handlers();

    boost::asio::ip::tcp::resolver resolver(_acceptor.get_io_service());
    boost::asio::ip::tcp::resolver::query query("0.0.0.0", std::to_string(port));
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
    _acceptor.open(endpoint.protocol());
    _acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    _acceptor.bind(endpoint);
    _acceptor.listen();

//    exec_accept();
}

listener::~listener()
{
//    std::cout << "req: " << request::n << std::endl
//              << "resp: " << response::n << std::endl
//              << "connection finished: " << connection::n << std::endl;
//    std::cout << "cache hits:";
}

void listener::run()
{
    std::cout << "Listening :" << _port << std::endl;
    prefork(_workers_count);
}

void listener::prefork(int workers_count)
{
    if (workers_count <= 0) {
        return;
    }

    _io_service.notify_fork(boost::asio::io_service::fork_prepare);
    pid_t pid = fork();
    if (pid < 0) {
        // error
    } else if (pid == 0) {
        // child
        _io_service.notify_fork(boost::asio::io_service::fork_child);


        _signals.cancel();
        exec_accept();
    } else {
        // parent
        _io_service.notify_fork(boost::asio::io_service::fork_parent);
        if (_acceptor.is_open())
            _acceptor.close();
        _forks.push_back(pid);
        std::cout << "Created worker with pid: " << pid << std::endl;
        prefork(workers_count - 1);
    }

    std::cout << "Starting io_service.... pid = " << getpid() << std::endl;
    _io_service.run();
    std::cout << "Finishing io_service.... pid = " << getpid() << std::endl;
}


void listener::exec_accept()
{
    auto connect = boost::make_shared<connection>(_io_service, _request_handler);
    _acceptor.async_accept(connect->socket(),
        [this, connect](boost::system::error_code ec) {
            if (!ec) {
                connect->run();
            }
            exec_accept();
        }
    );
}

void listener::init_signal_handlers()
{
    _signals.add(SIGINT);
    _signals.add(SIGTERM);
#ifdef SIGQUIT
    _signals.add(SIGQUIT);
#endif

    _signals.async_wait(
        [this](boost::system::error_code /*ec*/, int /*signo*/) {
            _io_service.stop();
            _connection.reset();
            for (auto pid : _forks) {
                ::kill(pid, SIGKILL);
            }
    });
}
