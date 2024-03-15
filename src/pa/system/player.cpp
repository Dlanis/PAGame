#include "player.hpp"
#include "pa/comp/area_response.hpp"
#include "pa/comp/camera.hpp"
#include "pa/comp/sprites.hpp"
#include "pa/comp/world.hpp"
#include <pa/comp/position.hpp>
#include <pa/comp/physical_body.hpp>
#include <pa/comp/physical_colliders.hpp>
#include <pa/comp/moving_body.hpp>
#include <pa/comp/player.hpp>
#include <pa/core/state.hpp>
#include <pa/core/config.hpp>

namespace pa::system {

void Player::update(core::State& state, const core::Config &config, const f64 delta_time) {
    glm::vec2 finish_position;
    state.get_registry().view<comp::World>().each([&finish_position](comp::World &world){ finish_position = world.finish_position; });

    auto player_view = state.get_registry().view<comp::Player, comp::Position, comp::MovingBody, comp::Camera, comp::AreaResponse, comp::Sprites>();

    for(auto entity : player_view) {
        auto &&[player, pos, mvbody, cam, arresp, sprites] = player_view.get(entity);

        player.timer += delta_time;
        
        glm::vec2 input;
        if(::IsKeyDown(KEY_LEFT)) {
            input.x -= 1;
        }
        if(::IsKeyDown(KEY_RIGHT)) {
            input.x += 1;
        }

        if(::IsKeyPressed(KEY_UP)) {
            player.wanted_jump_time = player.timer;
        }

        for(auto &spr : sprites()) {
            if(spr.id == player.main_sprite) {
                if(input.x > 0.01) {
                    spr.scale.x = 1;
                } else if (input.x < -0.01) {
                    spr.scale.x = -1;
                }
            }
        }
        
        bool on_floor{false};
        bool on_ceiling{false};

        for(auto &resp : arresp()) {
            if(resp.first == player.floor_trigger) {
                on_floor = resp.second;
            } else if(resp.first == player.ceiling_trigger) {
                on_ceiling = resp.second;
            }
        }

        if(on_ceiling) {
            mvbody.velocity.y = glm::max(mvbody.velocity.y, 0.f);
        }

        if(on_floor) {
            player.last_on_floor = player.timer;
        }

        bool can_jump{player.timer - player.last_on_floor < 0.1};

        if(can_jump && player.timer - player.wanted_jump_time < 0.1) {
            player.jump_time = player.timer;
        }

        if(::IsKeyDown(KEY_UP) && player.timer - player.jump_time < 0.2) {
            mvbody.velocity.y = glm::min(mvbody.velocity.y, -16 * 16.f);
        }
        
        if(on_floor && player.timer - player.jump_time > 0.1) {
            mvbody.velocity.y = glm::min(mvbody.velocity.y, 0.f);
        }

        mvbody.forced_velocity = input * glm::vec2(10 * 16.0);
        mvbody.acceleration = glm::vec2(0, 48 * 16.0);

        cam.target = pos.vector;
        cam.offset = {0.5, 0.5};
        
        // logt("y:{:>10.3f}", mvbody.velocity.y);
    }

}

}