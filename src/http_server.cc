#include "http_server.h"
#include "config/server_config.h"

#include <iostream>

http_server::http_server(int port)
    : _port(port),
      _listener(nullptr)
{

}

http_server::http_server(const std::string& config_path)
    : _listener(nullptr)
{
    server_config::set_config_path(config_path);
    _port = server_config::instance().get_port();
}

http_server::~http_server()
{
    delete _listener;
}

void http_server::run()
{
    std::cout << "Starting server..." << std::endl;
    auto conf = server_config::instance();
    _listener = new listener(conf.get_port(), 4);
    _listener->run();
    std::cout << "Server stopped" << std::endl;
}

void http_server::stop()
{

}
