#include "config.hpp"
#include <exception>
#include <fstream>
#include <iostream>

Config Config::read_config() {
    std::ifstream ifs(CONFIG_PATH);
    std::string str(std::istreambuf_iterator<char>{ifs}, {});
    std::cout << "config is: " << str << std::endl;

    try {
        nlohmann::json j = nlohmann::json::parse(str);
        return j.template get<Config>();
    } catch (std::exception& e) {
        std::cerr << "Couldn't read config from file" << std::endl;
        return Config();
    }
}
