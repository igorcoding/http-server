#include "http_server.h"

#include <iostream>
#include <boost/program_options.hpp>
#include <string>

int main(int argc, char** argv)
{
    namespace po = boost::program_options;
    po::options_description desc("Allowed options");
    std::string config_location;
    desc.add_options()
        ("help,h", "Produce help message")
        ("config,c", po::value<std::string>(&config_location)->default_value("./http.conf"), "Server config location")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 1;
    }

    if (!vm.count("config")) {
        std::cout << "Using default config." << std::endl;
    } else {
        std::cout << "Using config: " << config_location << std::endl;
    }
    std::cout << std::endl;

    try {
        http_server server(config_location);
        server.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }


    return 0;
}
