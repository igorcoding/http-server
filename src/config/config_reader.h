#ifndef CONFIG_READER_H
#define CONFIG_READER_H

#include <string>
#include <boost/property_tree/ptree.hpp>

class config_reader
{
public:
    static void read(const std::string& src, boost::property_tree::ptree* out_tree);

private:
    config_reader() = delete;
    config_reader(const config_reader& c) = delete;
};

#endif // CONFIG_READER_H
