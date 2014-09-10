#ifndef CONNECTION_H
#define CONNECTION_H

#include "worker.h"

#include <ev++.h>
#include <vector>
#include <map>
#include <condition_variable>

class listener
{
public:
    listener(struct ev_loop* loop, int port, size_t workers_count);
    ~listener();

    void run();
    void stop();

private:
    void init_socket();
    size_t pick_worker();
    void listener_cb(ev::io& w, int revents);
    void sock_cb(ev::io& w, int revents);

private:
    int _port;
    int _sfd;
    struct ev_loop* _loop;
    ev::io* _io;
    size_t _workers_count;

    std::vector<worker*> _workers;
    std::vector<std::condition_variable*> _cvs;

    std::map<int, sck_t*> _socks;
};

#endif // CONNECTION_H
