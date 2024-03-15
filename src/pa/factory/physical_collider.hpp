#pragma once

#include <pa/common.hpp>
#include <pa/comp/physical_colliders.hpp>

namespace pa::factory {

class PhysicalCollider final {
    entt::registry &registry;
    entt::entity &entity;

public:
    PhysicalCollider(entt::registry &registry, entt::entity &entity) : registry(registry), entity(entity) {}

    struct Opts {
        physics::AABB box;
        physics::Mask mask;
    };

    core::ID operator()(Opts opts) {
        physics::Collider phcollider;
        phcollider.box = opts.box;
        phcollider.mask = opts.mask;
        registry.get_or_emplace<comp::PhysicalColliders>(entity)().push_back(phcollider);
        return phcollider.id;
    }
};

}