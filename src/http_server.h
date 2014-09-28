#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "listener.h"

#include <string>

class http_server
{
public:
    http_server(int port);
    http_server(const std::string& config_path);
    ~http_server();

    void run();
    void stop();

private:
    int _port;
    listener* _listener;
};

#endif // HTTP_SERVER_H
