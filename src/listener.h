#ifndef LISTENER_H
#define LISTENER_H

#include "http/request_handler.h"

#include <iostream>
#include <ev++.h>

class listener
{
public:
    listener(int port, size_t workers_count);
    ~listener();

    void run();

private:
    int create_socket() const;
    void accept_cb(ev::io& w, int revents);
    static void signal_cb(ev::sig& w, int revents);

private:
    ev::io _io;
    ev::sig _signal;
    int _listen_fd;

    int _port;
    int _workers_count;

    request_handler* _req_handler;

    static const int _QUEUE_SIZE = 5;
};

#endif // LISTENER_H
