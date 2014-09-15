#ifndef SERVER_CONFIG_H
#define SERVER_CONFIG_H

#include <string>

class server_config
{
public:
    static void set_config_path(const std::string& path);
    static server_config& instance();

    const std::string& get_ip() const;
    int get_port() const;
    const std::string& get_document_root() const;
    const std::string& get_index_filename() const;
    size_t get_workers_count() const;
    double get_cache_period() const;

private:
    server_config();
    server_config(const std::string& ip, int port, const std::string& doc_root, const std::string& index_filename, size_t workers_count, double cache_period);
    static server_config& default_config();
    void load_config();

private:
    static std::string _config_path;

    std::string _ip;
    int _port;
    std::string _doc_root;
    std::string _index_filename;
    size_t _workers_count;
    double _cache_period;
};

#endif // SERVER_CONFIG_H
