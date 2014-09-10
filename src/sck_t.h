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
    sck_t(int sfd, struct ev_loop* loop)
        : _io(new ev::io(loop)),
          _req(new request()),
          _resp(new response()),
          _done(false)
    {
        fcntl(sfd, F_SETFL, fcntl(sfd, F_GETFL, 0) | O_NONBLOCK);
        _sfd = sfd;
    }

    ~sck_t() {
        _io->stop();
        delete _io;
        close(_sfd);
        delete _req;
        delete _resp;
    }

    int sfd() {
        return _sfd;
    }

    ev::io* io() {
        return _io;
    }

    request* req() {
        return _req;
    }

    response* resp() {
        return _resp;
    }

    bool is_done() {
        return _done;
    }

    void mark_done() {
        _done = true;
    }

private:
    int _sfd;
    ev::io* _io;
    request* _req;
    response* _resp;
    bool _done;
};

#endif // SCK_T_H
