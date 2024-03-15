#include "app.hpp"
#include "pa/comp/world.hpp"
#include "pa/factory/area_collider.hpp"
#include "pa/factory/camera.hpp"
#include "pa/system/player.hpp"
#include <pa/core/config.hpp>
#include <pa/core/resource_manager.hpp>
#include <pa/core/state.hpp>
#include <pa/factory/drawable.hpp>
#include <pa/factory/moving_body.hpp>
#include <pa/factory/physical_body.hpp>
#include <pa/factory/physical_collider.hpp>
#include <pa/factory/player.hpp>
#include <pa/factory/position.hpp>
#include <pa/factory/sprite.hpp>
#include <pa/physics/layer.hpp>
#include <pa/system/physics.hpp>
#include <pa/system/render.hpp>
#include <pa/core/world_loader.hpp>

namespace pa::core {

void App::run() {
    Config config{"PA.json"};
    State state;
    system::Render sys_render;
    system::Physics sys_physics;
    system::Player sys_player;
    
    state.get_resource_manager().init(config);
    sys_render.init(config);

    // {
    //     auto entity = state.get_registry().create();
    //     factory::Drawable{state.get_registry(), entity}({
    //         .visible = true
    //     });
    //     factory::Position{state.get_registry(), entity}({
    //         .vector = {0.0, 0.0}
    //     });
    //     factory::Sprite{state.get_registry(), entity}({
    //         .texture_id = state.get_resource_manager().load_texture("test_block.png"),
    //         .origin = {0.5, 0.5},
    //         .scale = {2.0, 2.0}
    //     });
    //     factory::PhysicalBody{state.get_registry(), entity}({});
    //     factory::PhysicalCollider{state.get_registry(), entity}({
    //         .box = {{-16, -16}, {32, 32}},
    //         .mask = {physics::Mask(1 << u8(physics::Layer::Player))},
    //     });
    //     factory::MovingBody{state.get_registry(), entity}({
    //         .forced_velocity = {0, 20}
    //     });
    // }

    {
        auto entity = state.get_registry().create();

        WorldLoader loader{state.get_registry(), entity};
        loader.load({.state = state, .m_resources_dirs = config.get_resources_dirs(), .filename = "test.tmj"});
    }

    {
        glm::vec2 player_pos;
        state.get_registry().view<comp::World>().each([&player_pos](comp::World &wrld){ player_pos = wrld.player_position; });

        auto entity = state.get_registry().create();
        factory::Drawable{state.get_registry(), entity}({
            .visible = true,
        });
        factory::Position{state.get_registry(), entity}({
            .vector = player_pos,
        });
        core::ID main_sprite = factory::Sprite{state.get_registry(), entity}({
            .texture_id = state.get_resource_manager().load_texture("faceball.png"),
            .origin = {0.5, 0.5},
            .scale = {1.0, 1.0},
        });
        factory::PhysicalBody{state.get_registry(), entity}({});

        factory::PhysicalCollider{state.get_registry(), entity}({
            .box = {{-9, -9}, {18, 18}},
            .mask = physics::create_mask({physics::Layer::Player}),
        });
        factory::MovingBody{state.get_registry(), entity}({});
        factory::Camera{state.get_registry(), entity}({
            .make_current = true,
            .zoom = 1.5,
        });
        core::ID floor_trigger = factory::AreaCollider{state.get_registry(), entity}({
            .box = {{-9 + 0.05/2, -9 + 0.01}, {18 - 0.05, 18}},
            .mask = physics::create_mask({physics::Layer::StaticEnviroment}),
        });
        core::ID ceiling_trigger = factory::AreaCollider{state.get_registry(), entity}({
            .box = {{-9 + 0.05/2, -9 - 0.01}, {18 - 0.05, 18}},
            .mask = physics::create_mask({physics::Layer::StaticEnviroment}),
        });
        factory::Player{state.get_registry(), entity}({
            .floor_trigger = floor_trigger,
            .ceiling_trigger = ceiling_trigger,
            .main_sprite = main_sprite,
        });
    }

    f64 delta_time{};

    bool should_close{false};

    while(!should_close) {
        delta_time = glm::mix(delta_time, glm::min(config.get_max_delta_time(), f64(GetFrameTime())), 0.25);

        // state.get_registry().view<comp::Position, comp::Sprites>().each([&delta_time](comp::Position &position, comp::Sprites &sprites){
        //     position.vector.x += 9.4*delta_time;
        //     for(auto &sprite : sprites()) {
        //         sprite.rotation += 30.0*delta_time;
        //     }
        // });

        sys_physics.update(state, config, delta_time);
        sys_player.update(state, config, delta_time);
        sys_render.update(state, config, delta_time);

        should_close = ::WindowShouldClose();

        if(should_close) {
            break;
        }
    }

    state.get_resource_manager().terminate();
    sys_render.terminate();
}
}