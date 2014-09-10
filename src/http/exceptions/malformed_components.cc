#include "malformed_components.h"

malformed_request::malformed_request()
    : std::exception()
{
}


unknown_method::unknown_method()
    : malformed_request()
{

}


malformed_uri::malformed_uri()
    : malformed_request()
{

}

malformed_protocol::malformed_protocol()
    : malformed_request()
{

}

malformed_header::malformed_header()
    : malformed_request()
{

}
