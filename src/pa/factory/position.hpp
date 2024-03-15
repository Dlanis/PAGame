#pragma once

#include <pa/common.hpp>
#include <pa/comp/position.hpp>

namespace pa::factory {

class Position final {
    entt::registry &registry;
    entt::entity &entity;

public:
    Position(entt::registry &registry, entt::entity &entity) : registry(registry), entity(entity) {}

    struct Opts {
        glm::vec2 vector;
    };

    void operator()(Opts opts) {
        comp::Position pos;
        pos.vector = opts.vector;
        registry.emplace<comp::Position>(entity, pos);
    }
};

}