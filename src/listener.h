#ifndef LISTENER_H
#define LISTENER_H

#include <iostream>

class listener
{
public:
    listener(int port, size_t workers_count);

    void run();
};

#endif // LISTENER_H
