#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include "request.h"
#include "response.h"
#include "../fs/file_reader.h"

class request_handler
{
public:
    static request_handler& instance();
    void handle(request* req, response* resp);


private:
    request_handler();
    request_handler(const request_handler& rh) = delete;
    file_reader _freader;
};

#endif // REQUEST_HANDLER_H
