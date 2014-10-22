#include "connection.h"

#include <unistd.h>
#include <fcntl.h>

connection::connection(int client_fd)
    : _client_fd(client_fd),
      _req(new request),
      _resp(new response)
{
    fcntl(_client_fd, F_SETFL, fcntl(_client_fd, F_GETFL, 0) | O_NONBLOCK);
    printf("Got connection\n");

    _io.set<connection, &connection::callback>(this);
    _io.start(_client_fd, ev::READ);
}

connection::~connection()
{
    if (!is_closed()) {
        close_connection();
    }

    delete _resp;
    _resp = nullptr;

    delete _req;
    _req = nullptr;
}

void connection::callback(ev::io& w, int revents)
{
    if (EV_ERROR & revents) {
        perror("got invalid event");
        return;
    }

    if (revents & EV_READ)
        read_cb(w);

    if (revents & EV_WRITE)
        write_cb(w);

    if (!_req->is_ready()) {
        _io.set(ev::READ);
    } else {
        _io.set(ev::READ|ev::WRITE);
    }
}

void connection::write_cb(ev::io& w)
{
    std::cout << "write_cb" << std::endl;
    if (!_req->is_ready()) {
        _io.set(ev::READ);
        return;
    }

    char msg[] = "HTTP/1.1 200 OK\r\n\r\nHello from server!";
    size_t s = strlen(msg);

    ssize_t written = ::send(w.fd, msg, s, 0);
    if (written < 0) {
        perror("write error");
        return;
    }

    std::cout << "Ended write_cb" << std::endl;
    _io.set(ev::READ);
    close_connection();

//    buffer->pos += written;
//    if (buffer->nbytes() == 0) {
//        write_queue.pop_front();
//        delete buffer;
//    }
}

// Receive message from client socket
void connection::read_cb(ev::io& w) {
    std::cout << "read_cb" << std::endl;

    char buffer[4096];

    ssize_t nread = ::recv(w.fd, buffer, sizeof(buffer), 0);

    if (nread < 0) {
        perror("read error");
        return;
    }

    if (nread == 0) {
        // Gack - we're deleting ourself inside of ourself!
        delete this;
    } else {
        _req->add_chunk(buffer, nread);
    }
}

void connection::close_connection()
{
    _io.stop();
    ::close(_client_fd);
    _connection_closed = true;
}

bool connection::is_closed() const {
    return _connection_closed;
}

