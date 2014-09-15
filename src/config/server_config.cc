#include "server_config.h"
#include "config_reader.h"

std::string server_config::_config_path = "";

server_config::server_config()
{
    load_config();
}

server_config::server_config(const std::string& ip,
                             int port,
                             const std::string& doc_root,
                             const std::string& index_filename,
                             size_t workers_count)
    : _ip(ip),
      _port(port),
      _doc_root(doc_root),
      _index_filename(index_filename),
      _workers_count(workers_count)
{ }

void server_config::set_config_path(const std::string& path)
{
    _config_path = path;
}

server_config& server_config::instance()
{
    static server_config i;
    return i;
}

const std::string& server_config::get_ip() const
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

const std::string&server_config::get_index_filename() const
{
    return _index_filename;
}

size_t server_config::get_workers_count() const
{
    return _workers_count;
}

void server_config::load_config()
{
    boost::property_tree::ptree tree;
    config_reader::read(_config_path, &tree);
    _ip = tree.get("ip", default_config()._ip);
    _port = tree.get("port", default_config()._port);
    _doc_root = tree.get("document_root", default_config()._doc_root);
    _index_filename = tree.get("index_filename", default_config()._index_filename);
    _workers_count = static_cast<size_t>(tree.get("workers_count", default_config()._workers_count));
}


server_config& server_config::default_config() {
    static server_config default_conf("127.0.0.1", 80, "./", "index.html", 8);
    return default_conf;
}
