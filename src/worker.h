#ifndef WORKER_H
#define WORKER_H

#include <boost/asio.hpp>
#include <thread>
#include <mutex>

class worker
{
public:
    worker();

private:
    void worker_func();

private:
    bool _working;
    boost::asio::io_service _io_service;
    std::thread _th;
    std::mutex _m;
};

#endif // WORKER_H
