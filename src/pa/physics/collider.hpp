#pragma once

#include <vector>
#include <pa/core/id.hpp>
#include <pa/physics/mask.hpp>
#include <pa/physics/aabb.hpp>
#include <pa/common.hpp>

namespace pa::physics {

struct Collider final {
    AABB box;
    Mask mask;

    core::ID id;
};

[[nodiscard]] inline std::optional<glm::vec2> penetration_vector_collider(
    const glm::vec2 &aposition, const physics::Collider &acollider,
    const glm::vec2 &bposition, const physics::Collider &bcollider)
{
    if((acollider.mask & bcollider.mask).none()) {
        return std::nullopt;
    }

    return physics::penetration_vector_aabb(acollider.box.offseted(aposition), bcollider.box.offseted(bposition));
}

[[nodiscard]] inline std::optional<glm::vec2> penetration_vector_colliders(
    const glm::vec2 &aposition, const std::vector<physics::Collider> &acolliders,
    const glm::vec2 &bposition, const std::vector<physics::Collider> &bcolliders)
{
    std::optional<glm::vec2> result{};
    
    for(auto &ac : acolliders) {
        for(auto &bc : bcolliders) {
            auto pen_vec{penetration_vector_collider(aposition, ac, bposition, bc)};
            if(!pen_vec.has_value()) continue;
            if(result.has_value()) {
                result->x = glm::abs(result->x) < glm::abs(pen_vec->x) ? pen_vec->x : result->x;
                result->y = glm::abs(result->y) < glm::abs(pen_vec->y) ? pen_vec->y : result->y;
            } else {
                result = pen_vec;
            }
        }
    }

    return result;
}

[[nodiscard]] inline bool check_collision_collider(
    const glm::vec2 &aposition, const physics::Collider &acollider,
    const glm::vec2 &bposition, const physics::Collider &bcollider)
{
    if((acollider.mask & bcollider.mask).none()) {
        return false;
    }

    return physics::check_collision_aabb(acollider.box.offseted(aposition), bcollider.box.offseted(bposition));
}

[[nodiscard]] inline bool check_collision_colliders(
    const glm::vec2 &aposition, const std::vector<physics::Collider> &acolliders,
    const glm::vec2 &bposition, const std::vector<physics::Collider> &bcolliders)
{
    for(auto &ac : acolliders) {
        for(auto &bc : bcolliders) {
            if(check_collision_collider(aposition, ac, bposition, bc)) {
                return true;
            }
        }
    }

    return false;
}

[[nodiscard]] inline std::optional<physics::Collider> merge_colliders(const std::vector<physics::Collider> &colliders) {
    if(colliders.empty()) {
        return std::nullopt;
    }

    glm::vec2 min{std::numeric_limits<f32>::max(), std::numeric_limits<f32>::max()};
    glm::vec2 max{std::numeric_limits<f32>::min(), std::numeric_limits<f32>::min()};
    physics::Mask merged_mask;

    for(auto &c : colliders) {
        min = glm::fmin(min, c.box.min());
        max = glm::fmax(max, c.box.max());
        merged_mask |= c.mask;
    }

    return {physics::Collider{.box{.position = min, .size = max - min}, .mask = merged_mask}};
}

}