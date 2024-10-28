#pragma once
#include <utility>
#include <nlohmann/json.hpp>

#define CONFIG_PATH "settings.json"

struct Config {
    std::pair<int, int> window_size;
    bool fullscreen;
    int max_fps;

    Config() :
        window_size({1280, 720}),
        fullscreen(false),
        max_fps(60)
    {}

    // Reads the config file at CONFIG_PATH and returns it.
    // If this file does not exist, instead it returns the default config.
    static Config read_config();

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Config, window_size, fullscreen, max_fps);
};
