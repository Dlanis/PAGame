#pragma once

#include <pa/common.hpp>
#include <pa/comp/area_colliders.hpp>

namespace pa::factory {

class AreaCollider final {
    entt::registry &registry;
    entt::entity &entity;

public:
    AreaCollider(entt::registry &registry, entt::entity &entity) : registry(registry), entity(entity) {}

    struct Opts {
        physics::AABB box;
        physics::Mask mask;
    };

    core::ID operator()(Opts opts) {
        physics::Collider arcollider;
        arcollider.box = opts.box;
        arcollider.mask = opts.mask;
        registry.get_or_emplace<comp::AreaColliders>(entity)().push_back(arcollider);
        return arcollider.id;
    }
};

}