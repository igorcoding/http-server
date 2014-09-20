#ifndef IO_SERVICE_MANAGER_H
#define IO_SERVICE_MANAGER_H

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <vector>
#include <thread>
#include <random>
#include <functional>

class io_service_manager
{
    typedef boost::shared_ptr<boost::thread> thread_ptr;
    typedef boost::shared_ptr<boost::asio::io_service> io_service_ptr;
    typedef boost::shared_ptr<boost::asio::io_service::work> work_ptr;

public:
    io_service_manager(size_t workers_count);

    void run();
    void stop();
    boost::asio::io_service& get_io_service();
    boost::asio::io_service& get_first_io_service();

private:
    size_t next_io();

private:
    size_t _workers_count;
    std::vector<io_service_ptr> _io_services;
    std::vector<work_ptr> _works;
    std::vector<thread_ptr> _threads;
    std::function<size_t()> _rand;
    size_t _current_io;
};

#endif // IO_SERVICE_MANAGER_H
