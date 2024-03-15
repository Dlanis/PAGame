#pragma once

#include "pa/core/id.hpp"
#include <pa/common.hpp>

namespace pa::comp {

struct Player final {
    core::ID floor_trigger;
    core::ID ceiling_trigger;
    core::ID main_sprite;
    f64 wanted_jump_time;
    f64 jump_time;
    f64 last_on_floor;
    f64 timer = 1000;
};

}