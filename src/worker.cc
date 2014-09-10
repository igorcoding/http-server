#include "worker.h"

#include <algorithm>
#include <iostream>

worker::worker(std::condition_variable& cv)
    : _running(false),
      _th(nullptr),
      _cv(cv),
      _loop(nullptr)
{ }

void worker::run()
{
    _th = new std::thread(std::bind(&worker::worker_func, this));
    _running = true;
}

void worker::add_socket(int sfd)
{
    _in_sockets.push_back(sfd);
}

void worker::worker_func()
{
    _loop = ev_loop_new();
    ev_run(_loop);

    while (_running) {
        std::cout << "Started worker. Waiting\n";

        std::unique_lock<std::mutex> lk(_m);
        _cv.wait(lk, [this]{ return !_in_sockets.empty() && _running; });

        std::cout << "Actual work begins\n";

        if (!_running) break;

        for (auto it = _in_sockets.begin(); it != _in_sockets.end(); ++it) {
            int sfd = *it;

            auto sock = new sck_t(sfd, _loop);
            sock->io()->set<worker, &worker::sock_cb>(this);
            sock->io()->start(sock->sfd(), ev::READ);

            __sockets.push_back(sock);
        }
        _in_sockets.clear();
        lk.unlock();

        std::cout << "Clearing up sockets. __sockets.size() = " << __sockets.size() << std::endl;

        for (auto it = __sockets.begin(); it != __sockets.end(); ) {
            if ((*it)->is_done()) {
                auto next_it = it;
                ++next_it;
                delete *it;
                __sockets.erase(it);
                it = next_it;
            } else {
                ++it;
            }
        }
    }
    ev_loop_destroy(_loop);
}

void worker::sock_cb(ev::io& w, int revents)
{
    w.stop();
    std::cout << "hi there!" << std::endl;
}


