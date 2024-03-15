#pragma once

#include <pa/common.hpp>
#include <pa/simple_gettersetter.hpp>

namespace pa::core {

class Config final {
    std::string window_title{"PAGame"};
    glm::ivec2  window_minimum_size{128, 96};
    glm::ivec2  window_default_size{640, 480};
    u32         window_flags{FLAG_WINDOW_RESIZABLE};

    std::vector<std::string> resources_dirs{"./assets"};

    f64 max_delta_time{0.1};
    i32 max_fps{300};

    glm::vec2 game_resolution{640, 480};

public:
    // Config() = delete;
    Config() {}
    Config(std::string_view filename);

    PA_SIMPLE_GETTERSETTER(window_title);
    PA_SIMPLE_GETTERSETTER(window_minimum_size);
    PA_SIMPLE_GETTERSETTER(window_default_size);
    PA_SIMPLE_GETTERSETTER(window_flags);

    PA_SIMPLE_GETTERSETTER(resources_dirs);
    
    PA_SIMPLE_GETTERSETTER(max_delta_time);
    PA_SIMPLE_GETTERSETTER(max_fps);

    PA_SIMPLE_GETTERSETTER(game_resolution);
};

}