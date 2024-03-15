#pragma once

#include <pa/common.hpp>

namespace pa::physics {

struct AABB {
    glm::vec2 position;
    glm::vec2 size;

    glm::vec2 min(glm::vec2 offset = {}) const {
        return position + offset;
    }

    glm::vec2 max(glm::vec2 offset = {}) const {
        return position + offset + size;
    }

    glm::vec2 center(glm::vec2 offset = {}) const {
        return position + (offset + size)/2.0f;
    }

    AABB offseted(glm::vec2 offset) const {
        AABB box{*this};
        box.position += offset;
        return box;
    }
};

[[nodiscard]] inline bool check_collision_aabb(const physics::AABB a, const physics::AABB b) {
	i8 d0 = b.max().x < a.min().x;
	i8 d1 = a.max().x < b.min().x;
	i8 d2 = b.max().y < a.min().y;
	i8 d3 = a.max().y < b.min().y;
	return !(d0 | d1 | d2 | d3);
}

[[nodiscard]] inline std::optional<glm::vec2> penetration_vector_aabb(const physics::AABB a, const physics::AABB b) {
    glm::vec2 dist = b.center() - a.center();
    glm::vec2 pen = (b.size + a.size)*0.5f - glm::abs(dist);

    if((pen.x < 0) | (pen.y < 0)) return std::nullopt;

    pen += glm::vec2(0.001);

    glm::vec2 delta = glm::sign(dist) * pen;
    
    if(pen.x < pen.y) {
        return {{delta.x, 0}};
    } else {
        return {{0, delta.y}};
    }
}

}