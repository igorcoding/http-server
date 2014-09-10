#include "listener.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <iostream>

listener::listener(struct ev_loop* loop, int port, size_t workers_count)
    : _port(port),
      _sfd(-1),
      _loop(loop),
      _io(new ev::io(_loop)),
      _workers_count(workers_count)
{
//    if (workers_count == 0) {
//        std::cerr << "Invalid workers count\n";
//        exit(1);
//    }
//    _cvs.resize(workers_count, nullptr);
//    _workers.resize(workers_count, nullptr);

//    for (size_t i = 0; i < workers_count; ++i) {
//        _cvs[i] = new std::condition_variable();
//        _workers[i] = new worker(*_cvs[i]);
//    }
}

listener::~listener()
{
    stop();
    for (auto& s : _socks)
        delete s.second;
}

void listener::run()
{
//    for (size_t i = 0; i < _workers_count; ++i) {
//        _workers[i]->run();
//    }

    init_socket();
    _io->set<listener, &listener::listener_cb>(this);
    _io->start(_sfd, ev::READ);
}

void listener::stop()
{
    _io->stop();
    delete _io;

    for (size_t i = 0; i < _workers_count; ++i) {
        delete _cvs[i];
        delete _workers[i];
    }
}

void listener::init_socket()
{
    int status = -1;

    _sfd = ::socket(AF_INET, SOCK_STREAM, 0);

    if (_sfd == -1) {
        std::cerr << "socket creation error " << std::endl;
        exit(1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);


    std::cout << "Binding socket..."  << std::endl;
    // we use to make the setsockopt() function to make sure the port is not in use
    // by a previous execution of our code.
    int yes = 1;
    status = ::setsockopt(_sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    status = ::bind(_sfd, (struct sockaddr*) &addr, sizeof(addr));
    if (status == -1) {
        std::cerr << "bind error" << std::endl;
        exit(1);
    }

    fcntl(_sfd, F_SETFL, fcntl(_sfd, F_GETFL, 0) | O_NONBLOCK);

    std::cout << "Listening :" << _port << std::endl;
    if (listen(_sfd, 5) == -1) {
        std::cerr << "listen error" << std::endl;
        exit(1);
    }
}

size_t listener::pick_worker()
{
    // TODO: some algorithm
    return 0;
}

void listener::listener_cb(ev::io& w, int revents)
{
    if (EV_ERROR & revents) {
        std::cerr << "got invalid event" << std::endl;
        exit(1);
    }

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_sfd = ::accept(w.fd, (struct sockaddr *) &client_addr, &client_len);

    if (client_sfd < 0) {
        std::cout << "accept error" << std::endl ;
        exit(1);
    }

//    auto worker_id = pick_worker();
//    _workers[worker_id]->add_socket(client_sfd);
//    _cvs[worker_id]->notify_one();

    auto sock = new sck_t(client_sfd, _loop);
    _socks[client_sfd] = sock;
    sock->io()->set<listener, &listener::sock_cb>(this);
    sock->io()->start(sock->sfd(), ev::READ);
}

void listener::sock_cb(ev::io& w, int revents)
{
    const int size = 1024;
    char buffer[size];
    ssize_t read;

    if (EV_ERROR & revents) {
        perror("got invalid event");
        return;
    }

    // Receive message from client socket
    read = recv(w.fd, buffer, size, 0);

    if(read < 0) {
        perror("read error");
        return;
    }

    if(read == 0) {
        // Stop and free watchet if client socket is closing
        w.stop();
        _socks.erase(w.fd);
        perror("peer might closing");
        return;
    } else {
        int cmp = memcmp(buffer + read - 4, "\r\n\r\n", 4);
        if (cmp == 0) {
            sck_t* sock = _socks[w.fd];
            sock->req()->parse(std::string(buffer, read));
            sock->resp()->set_status(status_codes::OK);
            auto data = sock->req()->get_raw();
            sock->resp()->assign_data(data.c_str(), data.length());

            auto resp = sock->resp()->build();
            send(w.fd, resp.c_str(), resp.length(), 0);
        }
    }
}
