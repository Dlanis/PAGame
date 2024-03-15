#pragma once

#include <pa/common.hpp>
#include <pa/comp/camera.hpp>

namespace pa::factory {

class Camera final {
    entt::registry &registry;
    entt::entity &entity;

public:
    Camera(entt::registry &registry, entt::entity &entity) : registry(registry), entity(entity) {}
    
    struct Opts {
        bool make_current{false};
        glm::vec2 offset{};  /// Camera offset (displacement from target)
        glm::vec2 target{};  /// Camera target (rotation and zoom origin)
        f32 rotation{};      /// Camera rotation in degrees
        f32 zoom{1.0};       /// Camera zoom (scaling), should be 1.0f by default
    };

    void operator()(Opts opts) {
        comp::Camera camera;
        if(opts.make_current) camera.make_current();
        camera.offset = opts.offset;
        camera.target = opts.target;
        camera.rotation = opts.rotation;
        camera.zoom = opts.zoom;
        registry.emplace<comp::Camera>(entity, std::move(camera));
    }
};

}