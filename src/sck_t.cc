#include "sck_t.h"

sck_t::sck_t(int sfd, struct ev_loop* loop)
        : _io(new ev::io(loop)),
          _req(new request()),
          _resp(new response()),
          _done(false)
{
    fcntl(sfd, F_SETFL, fcntl(sfd, F_GETFL, 0) | O_NONBLOCK);
    _sfd = sfd;
}

sck_t::~sck_t() {
    _io->stop();
    delete _io;
    close(_sfd);
    delete _req;
    delete _resp;
}

int sck_t::sfd() {
    return _sfd;
}

ev::io* sck_t::io() {
    return _io;
}

request* sck_t::req() {
    return _req;
}

response* sck_t::resp() {
    return _resp;
}

bool sck_t::is_done() {
    return _done;
}

void sck_t::mark_done() {
    _done = true;
}