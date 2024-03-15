#pragma once

#include <pa/common.hpp>
#include <pa/comp/physical_body.hpp>

namespace pa::factory {

class PhysicalBody final {
    entt::registry &registry;
    entt::entity &entity;

public:
    PhysicalBody(entt::registry &registry, entt::entity &entity) : registry(registry), entity(entity) {}
    
    struct Opts {
        f32 friction{0.0};
    };

    void operator()(Opts opts) {
        comp::PhysicalBody phbody;
        phbody.friction = opts.friction;
        registry.emplace<comp::PhysicalBody>(entity, phbody);
    }
};

}