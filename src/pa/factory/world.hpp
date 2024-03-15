#pragma once

#include <pa/common.hpp>
#include <pa/comp/world.hpp>

namespace pa::factory {

class World final {
    entt::registry &registry;
    entt::entity &entity;

public:
    World(entt::registry &registry, entt::entity &entity) : registry(registry), entity(entity) {}

    struct Opts {
        glm::vec2 player_position;
        glm::vec2 finish_position;
    };

    void operator()(Opts opts) {
        comp::World world;
        world.player_position = opts.player_position;
        world.finish_position = opts.finish_position;
        registry.emplace<comp::World>(entity, world);
    }
};

}