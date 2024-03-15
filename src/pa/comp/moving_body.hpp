#pragma once

#include <pa/common.hpp>

namespace pa::comp {

struct MovingBody final {
    glm::vec2 velocity;
    glm::vec2 forced_velocity;
    glm::vec2 acceleration;
    bool sliding;
};

}