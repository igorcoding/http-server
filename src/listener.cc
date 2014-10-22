#include "listener.h"
#include "connection.h"

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

listener::listener(int port, size_t workers_count)
    : _io(),
      _signal(),
      _listen_fd(-1),
      _port(port),
      _workers_count(workers_count),
      _req_handler(new request_handler)
{
    _listen_fd = create_socket();
    _io.set<listener, &listener::accept_cb>(this);
    _io.set(_listen_fd, ev::READ | ev::WRITE);

    _signal.set<&listener::signal_cb>();
    _signal.set(SIGINT);
    _signal.set(SIGABRT);
#ifdef SIGQUIT
    _signal.set(SIGQUIT);
#endif //SIGQUIT
}

listener::~listener()
{
    ::close(_listen_fd);

    _signal.stop();
    _io.stop();

}

void listener::run()
{
    ev::default_loop _loop;

    _signal.start();
    _io.start();
    _loop.run();
}

int listener::create_socket() const
{
    struct sockaddr_in serv_addr;
    int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

    if (sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        ::exit(1);
    }

    int yes = 1;
    if (::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0) {
        std::cerr << "Error setting socket options" << std::endl;
        ::exit(1);
    }

    ::bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(_port);

    if (::bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Bind error" << std::endl;
        ::exit(1);
    }

    if (listen(sockfd, _QUEUE_SIZE) < 0) {
        std::cerr << "Listen error" << std::endl;
        ::exit(1);
    }

    std::cout << "Listening sockfd = " << sockfd << std::endl;

    return sockfd;
}

void listener::accept_cb(ev::io& w, int revents)
{
    std::cout << "connected" << std::endl;

    if (EV_ERROR & revents) {
        perror("got invalid event");
        return;
    }

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int client_fd = accept(w.fd, (struct sockaddr *) &client_addr, &client_len);

    if (client_fd < 0) {
        perror("accept error");
        return;
    }

    auto conn = new connection(client_fd);
//    _io.start();

//    const int size = 8192;
//    char buffer[size];
//    bzero(buffer,size);
//    ssize_t n = recv(newsockfd, buffer, size, 0);
//    if (n < 0) printf("ERROR reading from socket");
//    printf("Here is the message: %s\n", buffer);

//    request* req = new request;
//    response* resp = new response;

//    req->add_chunk(buffer, n);
//    _req_handler->handle(req, resp);

//    auto status_line = resp->get_status_line();
//    n = send(newsockfd, status_line.c_str(), status_line.length(), 0);
//    if (n < 0) printf("ERROR writing to socket");
//    n = send(newsockfd, resp->get_data(), resp->get_data_size(), 0);
//    if (n < 0) printf("ERROR writing to socket");

//    delete req;
//    req = nullptr;
//    delete resp;
//    resp = nullptr;

//    close(newsockfd);
}

void listener::signal_cb(ev::sig& w, int revents)
{
    w.loop.break_loop();
    std::cout << "signalled" << std::endl;
}
