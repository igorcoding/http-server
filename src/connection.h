#ifndef CONNECTION_H
#define CONNECTION_H

#include "http/request.h"
#include "http/response.h"

#include <ev++.h>

class connection
{
public:
    connection(int client_fd);
    ~connection();

    bool is_closed() const;

private:
    void callback(ev::io& w, int revents);
    void write_cb(ev::io& w);
    void read_cb(ev::io& w);

    void close_connection();

private:
    int _client_fd;
    ev::io _io;
    bool _connection_closed;

    request* _req;
    response* _resp;
};

#endif // CONNECTION_H
