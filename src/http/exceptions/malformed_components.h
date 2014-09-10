#ifndef MALFORMED_COMPONENTS_H
#define MALFORMED_COMPONENTS_H

#include <stdexcept>

class malformed_request : public std::exception
{
public:
    malformed_request();
};

class unknown_method : public malformed_request
{
public:
    unknown_method();
};

class malformed_uri : public malformed_request
{
public:
    malformed_uri();
};

class malformed_protocol : public malformed_request
{
public:
    malformed_protocol();
};

class malformed_header : public malformed_request
{
public:
    malformed_header();
};


#endif // MALFORMED_COMPONENTS_H
