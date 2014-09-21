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
                             size_t workers_count,
                             double cache_period)
    : _ip(ip),
      _port(port),
      _doc_root(doc_root),
      _index_filename(index_filename),
      _workers_count(workers_count),
      _cache_period(cache_period)
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

double server_config::get_cache_period() const
{
    return _cache_period;
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
    _cache_period = tree.get("cache_period", default_config()._cache_period);
}


server_config& server_config::default_config() {
    static server_config default_conf("127.0.0.1", 80, "./", "index.html", 8, 60);
    return default_conf;
}

std::ostream& operator <<(std::ostream& os, const server_config& config)
{
    os << "ip: " << config._ip << std::endl
       << "port: " << config._port << std::endl
       << "document root: " << config._doc_root << std::endl
       << "index filename: " << config._index_filename << std::endl
       << "workers count: " << config._workers_count << std::endl
       << "cache period (min): " << config._cache_period << std::endl;
    return os;
}
