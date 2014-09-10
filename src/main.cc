#include "http_server.h"

#include <iostream>

int main()
{
    http_server server("/home/igor/Projects/cpp/http-server/test_config.json");
    server.run();
    return 0;
}
