#ifndef WORKER_H
#define WORKER_H

#include "sck_t.h"

#include <vector>
#include <list>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <fcntl.h>
#include <unistd.h>
#include <ev++.h>


class worker
{
public:
    worker(std::condition_variable& cv);
    void run();
    void add_socket(int sfd);

private:
    void worker_func();
    void sock_cb(ev::io& w, int revents);

private:
    bool _running;
    std::thread* _th;
    std::condition_variable& _cv;
    std::mutex _m;
    std::vector<int> _in_sockets;
    std::list<sck_t*> __sockets;
    struct ev_loop* _loop;
};

#endif // WORKER_H
