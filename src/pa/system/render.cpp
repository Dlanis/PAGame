#include "render.hpp"
#include "pa/comp/area_colliders.hpp"
#include "pa/comp/physical_body.hpp"
#include "pa/comp/physical_colliders.hpp"

#include <pa/comp/drawable.hpp>
#include <pa/comp/position.hpp>
#include <pa/comp/sprites.hpp>
#include <pa/core/config.hpp>
#include <pa/core/state.hpp>
#include <pa/comp/camera.hpp>

namespace pa::system {

Render::~Render() {
    if(m_inited) {
        loge("destructed before terminalization.");
    }
}

void Render::init(const core::Config& config) {
    auto& win_siz{config.get_window_default_size()};
    auto& win_siz_min{config.get_window_minimum_size()};
    auto& win_title{config.get_window_title()};
    auto& win_flags{config.get_window_flags()};
    auto& max_fps{config.get_max_fps()};

    ::SetConfigFlags(win_flags);
    ::InitWindow(win_siz.x, win_siz.y, win_title.c_str());
    ::SetWindowMinSize(win_siz_min.x, win_siz_min.y);
    ::SetExitKey(-1);
    ::SetTargetFPS(max_fps);

    m_inited = true;
}

void Render::terminate() {
    ::CloseWindow();

    m_inited = false;
}

void Render::update(core::State& state, const core::Config &config, const f64 delta_time) {
    if(m_inited && ::IsWindowReady()) {
        ::BeginDrawing();
        ::ClearBackground(BLACK);

        /// default camera
        const Camera2D default_camera{
            {::GetRenderWidth()/2.0f, ::GetRenderHeight()/2.0f},
            {0.0, 0.0},
            0.0,
            glm::min(float(::GetRenderWidth())/config.get_game_resolution().x,
                     float(::GetRenderHeight())/config.get_game_resolution().y),
        };

        Camera2D camera;

        /// Setting Camera
        {
            auto camera_view(state.get_registry().view<comp::Camera>());

            /// Searching for current camera in O(n) time.
            /// I probably don't need thousands of cameras
            std::pair<f64, comp::Camera*> max{-1.0, nullptr};

            for(auto entity : camera_view) {
                auto &&[camera] = camera_view.get(entity);
                
                if(camera.get_current_time() > max.first) {
                    max = {camera.get_current_time(), &camera};
                }
            }

            if(max.second != nullptr) {
                auto &offset{max.second->offset};
                auto &target{max.second->target};
                camera = {
                    {GetRenderWidth()*offset.x, GetRenderHeight()*offset.y}, 
                    {target.x, target.y},
                    max.second->rotation,
                    default_camera.zoom * max.second->zoom,
                };
            } else {
                camera = default_camera;
            }
        }

        ::BeginMode2D(camera);

        /// Sprites Rendering
        {
            auto sprites_view{state.get_registry().view<comp::Position, comp::Drawable, comp::Sprites>()};

            for(auto entity : sprites_view) {
                auto &&[position, drawable, sprites] = sprites_view.get(entity);

                for(auto sprite : sprites()) {
                    if(!sprite.texture_id.has_value()) {
                        logt("Rendering sprite at position {:.5f},{:.5f} failed. It doesn't have texture.", position.vector.x, position.vector.y);
                        continue;
                    }

                    auto texture{state.get_resource_manager().get_texture(sprite.texture_id.value())};

                    if(!texture.has_value()) {
                        logt("Rendering sprite at position {:.5f},{:.5f} failed. It doesn't have texture.", position.vector.x, position.vector.y);
                        continue;
                    }

                    i32 texture_width{texture->get()->width};
                    i32 texture_height{texture->get()->height};
                    
                    Rectangle source{sprite.src.x     * texture_width, sprite.src.y      * texture_height, 
                                     sprite.src.width * texture_width, sprite.src.height * texture_height};
                    Rectangle destination{position.vector.x + sprite.offset.x, position.vector.y + sprite.offset.y,
                                          source.width * sprite.scale.x, source.height * sprite.scale.x};
                    Vector2 origin{destination.width * sprite.origin.x, destination.height * sprite.origin.y};

                    ::DrawTexturePro(*texture->get(), source, destination, origin, sprite.rotation, sprite.tint);
                }
            }
        }

        if(false){
            auto physical_body_view{state.get_registry().view<comp::Position, comp::PhysicalBody, comp::PhysicalColliders>()};
            
            physical_body_view.each([](comp::Position &pos, comp::PhysicalBody &phb, comp::PhysicalColliders &phcs){
                for(auto &phc : phcs()) {
                    ::Color cl{0x59, 0x89, 0xff, u8(0.25f*255)};
                    ::Rectangle rec{
                        phc.box.offseted(pos.vector).position.x,
                        phc.box.offseted(pos.vector).position.y,
                        phc.box.size.x,
                        phc.box.size.y,
                    };
                    ::DrawRectangleRec(rec, cl);
                }
            });
        }

        if(false){
            auto area_colliders_view{state.get_registry().view<comp::Position, comp::AreaColliders>()};
            
            area_colliders_view.each([](comp::Position &pos, comp::AreaColliders &arcls){
                for(auto &arcl : arcls()) {
                    ::Color cl{0x35, 0xad, 0x00, u8(0.25f*255)};
                    ::Rectangle rec{
                        arcl.box.offseted(pos.vector).position.x,
                        arcl.box.offseted(pos.vector).position.y,
                        arcl.box.size.x,
                        arcl.box.size.y,
                    };
                    ::DrawRectangleRec(rec, cl);
                }
            });
        }

        ::EndMode2D();

        ::DrawText(fmt::format("{:.2f}\n{:.4f}", 1.0/::GetFrameTime(), delta_time).c_str(), 5, 5, 10, WHITE);

        ::EndDrawing();
    }
}

}
