#pragma once

#include <pa/common.hpp>
#include <pa/comp/drawable.hpp>

namespace pa::factory {

class Drawable final {
    entt::registry &registry;
    entt::entity &entity;

public:
    Drawable(entt::registry &registry, entt::entity &entity) : registry(registry), entity(entity) {}

    struct Opts {
        bool visible{true};
    };

    void operator()(Opts opts) {
        comp::Drawable drawable;
        drawable.visible = opts.visible;
        registry.emplace<comp::Drawable>(entity, drawable);
    }
};

}