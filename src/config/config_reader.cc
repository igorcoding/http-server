#include "config_reader.h"
#include "config_read_error.h"

#include <boost/property_tree/json_parser.hpp>
#include <fstream>

void config_reader::read(const std::string& src, boost::property_tree::ptree* out_tree)
{
    std::ifstream ifs;
    ifs.open(src);

    if (!ifs.is_open()) {
        throw config_read_error("File not found");
    }

    try {
        boost::property_tree::read_json(ifs, *out_tree);
    } catch (std::exception& e) {
        throw config_read_error(e.what());
    }
}

