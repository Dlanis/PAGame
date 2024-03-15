#include "config.hpp"

#include <glaze/glaze.hpp>

template<>
struct glz::meta<pa::core::Config> {
    using T = pa::core::Config;
    static constexpr auto value = object(
        "window_title", custom<&T::set_window_title, &T::get_window_title>, "Window title"_c,
        "window_minimum_size", custom<&T::set_window_minimum_size, &T::get_window_minimum_size>, "Minimum window size"_c,
        "window_default_size", custom<&T::set_window_default_size, &T::get_window_default_size>, "Default window size"_c,
        "window_flags", custom<&T::set_window_flags, &T::get_window_flags>, "Window creation flags"_c,
        "resources_dirs", custom<&T::set_resources_dirs, &T::get_resources_dirs>, "Assets directories in priority order"_c,
        "max_delta_time", custom<&T::set_max_delta_time, &T::get_max_delta_time>, "Maximum game delta time"_c,
        "max_fps", custom<&T::set_max_fps, &T::get_max_fps>, "Maximum FPS"_c,
        "game_resolution", custom<&T::set_game_resolution, &T::get_game_resolution>, "Game internal resolution"_c
        );
};

template<typename S, glm::qualifier Q>
struct glz::meta<glm::vec<2, S, Q>> {
    using T = glm::vec<2, S, Q>;
    static constexpr auto value = array(
        [](auto&& self) -> auto& { return self.x; },
        [](auto&& self) -> auto& { return self.y; }
    );
};

namespace pa::core {

Config::Config(std::string_view filename) {
    std::string buffer{};

    if(!fs::exists(filename)) {
        glz::write<glz::opts{.format = glz::json, .comments = true, .prettify = true, .indentation_width = 2}>(*this, buffer);
        glz::write_error ec = {glz::buffer_to_file(buffer, std::string{filename})};

        if(ec) {
            pa::logc("Failed to write config.\nerror_code: {}", i64(ec.ec));
            std::exit(1);
        }
    } else {
        auto ec = glz::read_file_json<glz::opts{.comments=true, .error_on_unknown_keys=true}>(*this, filename, buffer);
        
        if(ec) {
            pa::logc("Failed to load config.\n{}", glz::format_error(ec, buffer));
            std::exit(1);
        }
    }
}

}
