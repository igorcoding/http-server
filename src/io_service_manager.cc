#include "io_service_manager.h"

#include <boost/bind.hpp>
#include <boost/make_shared.hpp>

io_service_manager::io_service_manager(size_t workers_count)
    : _workers_count(workers_count),
      _current_io(0)
{
    for (size_t i = 0; i < _workers_count; ++i) {
        io_service_ptr io(new boost::asio::io_service);
        work_ptr w(new boost::asio::io_service::work(*io));
        _works.push_back(w);
        _io_services.push_back(io);
    }
}

void io_service_manager::run()
{
    for (size_t i = 0; i < _workers_count; ++i) {
        _threads.push_back(boost::make_shared<thread_t>(
                               boost::bind(&boost::asio::io_service::run, _io_services[i])));
    }
    std::cout << "Started " << _workers_count << " workers." << std::endl;

    for (auto th : _threads) {
        th->join();
    }
}

void io_service_manager::stop()
{
    for (auto io : _io_services) {
        io->stop();
    }
}

boost::asio::io_service& io_service_manager::get_io_service()
{
    auto& serv = *_io_services[_current_io];
    next_io();
    return serv;
}

size_t io_service_manager::next_io()
{
    ++_current_io;
    if (_current_io >= _io_services.size()) {
        _current_io = 0;
    }
    return _current_io;
}
