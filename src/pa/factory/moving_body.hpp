#pragma once

#include <pa/common.hpp>
#include <pa/comp/moving_body.hpp>

namespace pa::factory {

class MovingBody final {
    entt::registry &registry;
    entt::entity &entity;

public:
    MovingBody(entt::registry &registry, entt::entity &entity) : registry(registry), entity(entity) {}

    struct Opts {
        bool sliding{true};
        glm::vec2 velocity{0, 0};
        glm::vec2 forced_velocity{0, 0};
        glm::vec2 acceleration{0, 0};
    };

    void operator()(Opts opts) {
        comp::MovingBody mvbody;
        mvbody.sliding = opts.sliding;
        mvbody.velocity = opts.velocity;
        mvbody.forced_velocity = opts.forced_velocity;
        mvbody.acceleration = opts.acceleration;
        registry.emplace<comp::MovingBody>(entity, mvbody);
    }
};

}