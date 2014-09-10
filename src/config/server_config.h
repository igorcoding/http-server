#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class server_config
{
public:
    static void set_config_path(const std::string& path);
    static server_config& instance();

    const std::string& get_ip() const;
    int get_port() const;
    const std::string& get_document_root() const;

private:
    server_config();
    server_config(const std::string& ip, int port, const std::string& doc_root);
    static server_config& default_config();
    void load_config();

private:
    static std::string _config_path;

    std::string _ip;
    int _port;
    std::string _doc_root;
};

#endif // CONFIG_H
