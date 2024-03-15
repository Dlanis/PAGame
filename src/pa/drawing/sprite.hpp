#pragma once

#include <pa/common.hpp>
#include <pa/core/id.hpp>
#include <pa/resource_id/texture_id.hpp>

namespace pa::drawing {
    
struct Sprite final {
    core::ID id;
    std::optional<resource_id::TextureID> texture_id;
    Rectangle src;  /// in uv coords
    glm::vec2 offset;
    glm::vec2 origin;
    glm::vec2 scale;
    f64 rotation;
    Color tint;
};

}