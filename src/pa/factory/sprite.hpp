#pragma once

#include <pa/common.hpp>
#include <pa/comp/sprites.hpp>
#include <pa/resource_id/texture_id.hpp>

namespace pa::factory {

class Sprite final {
    entt::registry &registry;
    entt::entity &entity;

public:
    Sprite(entt::registry &registry, entt::entity &entity) : registry(registry), entity(entity) {}

    struct Opts {
        std::optional<resource_id::TextureID> texture_id;
        Rectangle src{0.0, 0.0, 1.0, 1.0};
        glm::vec2 offset{0.0, 0.0};
        glm::vec2 origin{0.0, 0.0};
        glm::vec2 scale{1.0, 1.0};
        f64 rotation{0.0};
        Color tint{255, 255, 255, 255};
    };

    core::ID operator()(Opts opts) {
        drawing::Sprite sprite;
        sprite.texture_id = opts.texture_id;
        sprite.src        = opts.src;
        sprite.offset     = opts.offset;
        sprite.origin     = opts.origin;
        sprite.scale      = opts.scale;
        sprite.rotation   = opts.rotation;
        sprite.tint       = opts.tint;
        registry.get_or_emplace<comp::Sprites>(entity)().push_back(sprite);
        return sprite.id;
    }
};

}