#ifndef SCK_T_H
#define SCK_T_H

#include "http/request.h"
#include "http/response.h"

#include <ev++.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

class sck_t
{
public:
    sck_t(int sfd, struct ev_loop* loop);
    ~sck_t();

    int sfd();
    ev::io* io();
    request* req();
    response* resp();
    bool is_done();
    void mark_done();

private:
    int _sfd;
    ev::io* _io;
    request* _req;
    response* _resp;
    bool _done;
};

#endif // SCK_T_H
