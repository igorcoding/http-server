#include "http_server.h"
#include "request.h"

#include <iostream>

int main()
{
    auto r = "GET /wiki/HTTP HTTP/1.0\r\nHost: ru.wikipedia.org\r\nContent-Type: text/plain; charset=windows-1251\r\nContent-Language: ru\r\n\r\n";
    auto req = request::parse(r);
    return 0;
}
