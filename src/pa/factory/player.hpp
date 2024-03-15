#pragma once

#include <pa/common.hpp>
#include <pa/comp/player.hpp>

namespace pa::factory {

class Player final {
    entt::registry &registry;
    entt::entity &entity;

public:
    Player(entt::registry &registry, entt::entity &entity) : registry(registry), entity(entity) {}

    struct Opts {
        core::ID floor_trigger;
        core::ID ceiling_trigger;
        core::ID main_sprite;
    };

    void operator()(Opts opts) {
        comp::Player player;
        player.floor_trigger = opts.floor_trigger;
        player.ceiling_trigger = opts.ceiling_trigger;
        player.main_sprite = opts.main_sprite;
        registry.emplace<comp::Player>(entity, player);
    }
};

}