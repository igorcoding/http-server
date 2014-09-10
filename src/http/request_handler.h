#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include "request.h"
#include "response.h"

class request_handler
{
public:
    static void handle(request* req, response* resp);


private:
    request_handler() = delete;
    request_handler(const request_handler& rh) = delete;
};

#endif // REQUEST_HANDLER_H