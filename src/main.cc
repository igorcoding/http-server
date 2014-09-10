#include "http_server.h"

#include <iostream>

int main()
{
    try {
        http_server server("/home/igor/Projects/cpp/http-server/test_config.json");
        server.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }


    return 0;
}
