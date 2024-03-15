#include "physics.hpp"
#include "pa/comp/area_colliders.hpp"
#include "pa/comp/area_response.hpp"
#include <pa/comp/position.hpp>
#include <pa/comp/physical_body.hpp>
#include <pa/comp/physical_colliders.hpp>
#include <pa/comp/moving_body.hpp>
#include <pa/core/state.hpp>
#include <pa/core/config.hpp>
#include <pa/physics/aabb.hpp>
#include <ankerl/unordered_dense.h>
#include <limits>

namespace pa::system {

void Physics::update(core::State& state, const core::Config &config, const f64 full_delta_time) {
    i8 iterations{8};

    ankerl::unordered_dense::map<entt::entity, physics::Collider> merged_collider_cache;
    
    {
        auto physical_collider_view{state.get_registry().view<comp::PhysicalColliders>()};
        for(auto entity : physical_collider_view) {
            auto &&[phcolliders] = physical_collider_view.get(entity);
            
            auto merged{merge_colliders(phcolliders())};

            if(merged.has_value()) {
                merged_collider_cache[entity] = merged.value();
            }
        }
    }

    auto physical_body_view{state.get_registry().view<comp::Position, comp::PhysicalBody, comp::PhysicalColliders>()};
    
    f32 delta = f32(full_delta_time) / f32(iterations);

    for(i8 i{}; i < iterations; i++) {

        auto moving_body_view{state.get_registry().view<comp::Position, comp::PhysicalBody, comp::PhysicalColliders, comp::MovingBody>()};

        for(auto mventity : moving_body_view) {
            auto&& [mvpos, mvph, mvcolliders, mvbody] = moving_body_view.get(mventity);

            mvbody.velocity += mvbody.acceleration * delta;

            glm::vec2 mv_velocity{
                glm::abs(mvbody.forced_velocity.x) > glm::abs(mvbody.velocity.x) ? mvbody.forced_velocity.x : mvbody.velocity.x,
                glm::abs(mvbody.forced_velocity.y) > glm::abs(mvbody.velocity.y) ? mvbody.forced_velocity.y : mvbody.velocity.y,
            };
            mv_velocity *= delta;
            mvpos.vector += mv_velocity;

            if(mvcolliders().empty()) continue;

            physics::Collider mv_move_collider;

            {
                auto oldbox{merged_collider_cache[mventity]};
                oldbox.box.position -= mv_velocity;
                mv_move_collider = physics::merge_colliders({oldbox, merged_collider_cache[mventity]}).value();
            }

            bool collided{false};

            for(auto obentity : physical_body_view) {
                if(obentity == mventity) continue;

                auto&& [obpos, obbody, obcolliders] = physical_body_view.get(obentity);

                if(obcolliders().empty()) continue;
                if(!physics::check_collision_collider(mvpos.vector, mv_move_collider, obpos.vector, merged_collider_cache[obentity])) continue;

                // if(physics::check_collision_colliders(mvpos.vector, mvcolliders(), obpos.vector, obcolliders())) {
                //     collided = true;
                // }
                auto pen_vec{physics::penetration_vector_colliders(mvpos.vector, mvcolliders(), obpos.vector, obcolliders()).value_or(glm::vec2{0, 0})};
                mvpos.vector -= pen_vec;

                // mvbody.velocity.y -= pen_vec.y/(delta + 1e-6);

                // /// FIXME: this is hack
                // if(glm::abs(pen_vec.y) > 1e-6) {
                //     mvbody.velocity.y = 0;
                // }

                // if(glm::abs(pen_vec.x) > 1e-6) {
                //     mvbody.velocity.x = 0;
                // }
            }

            if(collided) {
                mvpos.vector -= mv_velocity;
            }
        }
    }

    {
        auto area_colliders_view = state.get_registry().view<comp::Position, comp::AreaColliders>();

        for(auto acentity : area_colliders_view) {
            auto &&[acpos, accolliders] = area_colliders_view.get(acentity);

            // std::vector<std::pair<core::ID, bool>> response;
            comp::AreaResponse response;
            response().reserve(accolliders().size());

            for(auto &accollider : accolliders()) {
                bool collided{false};

                for(auto obentity : physical_body_view) {
                    auto&& [obpos, obbody, obcolliders] = physical_body_view.get(obentity);

                    if(obcolliders().empty()) continue;
                    collided |= physics::check_collision_colliders(acpos.vector, {accollider}, obpos.vector, obcolliders());
                }

                response().push_back({accollider.id, collided});
            }

            response().shrink_to_fit();
            
            state.get_registry().emplace_or_replace<comp::AreaResponse>(acentity, response);
        }
    }
}

}