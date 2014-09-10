#include "server_config.h"
#include "config_reader.h"

#include <boost/property_tree/ptree.hpp>

std::string server_config::_config_path = "";
server_config server_config::_default_config("127.0.0.1", 80, ".");

server_config::server_config()
{
    load_config();
}

server_config::server_config(const std::string& ip, int port, const std::string& doc_root)
    : _ip(ip),
      _port(port),
      _doc_root(doc_root)
{

}

void server_config::set_config_path(const std::string& path)
{
    _config_path = path;
}

server_config& server_config::instance()
{
    static server_config i;
    return i;
}

const std::string&server_config::get_ip() const
{
    return _ip;
}

int server_config::get_port() const
{
    return _port;
}

const std::string& server_config::get_document_root() const
{
    return _doc_root;
}

void server_config::load_config()
{
    boost::property_tree::ptree tree;
    config_reader::read(_config_path, &tree);
    _ip = tree.get("ip", _default_config._ip);
    _port = tree.get("port", _default_config._port);
    _doc_root = tree.get("document_root", _default_config._doc_root);
}


