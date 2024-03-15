#include "world_loader.hpp"
#include "pa/factory/drawable.hpp"
#include "pa/factory/physical_body.hpp"
#include "pa/factory/physical_collider.hpp"
#include "pa/factory/position.hpp"
#include "pa/factory/sprite.hpp"
// #include "pa/factory/world.hpp"
#include "pa/physics/layer.hpp"
#include <pa/comp/world.hpp>
#include <glaze/glaze.hpp>

namespace pa::core {

struct chunk_layer_t {
    std::vector<i8> data;
    i32 height;
    i32 id;
    std::string name;
    f32 opacity;
    std::string type;
    bool visible;
    i32 width;
    f32 x;
    f32 y;
};

struct object_t {
    i32 height;
    i32 id;
    std::string name;
    bool point;
    f32 rotation;
    std::string type;
    bool visible;
    i32 width;
    f32 x;
    f32 y;
};

struct object_layer_t {
    std::string draworder;
    i32 id;
    std::string name;
    std::vector<object_t> objects;
    f32 opacity;
    std::string type;
    bool visible;
    f32 x;
    f32 y;
};

struct tileset_t {
    i32 firstgid;
    std::string source;
};

struct tmj_t {
    bool infinite;
    i32 compressionlevel;
    i32 height;
    i32 width;
    i32 nextlayerid;
    i32 nextobjectid;
    i32 tileheight;
    i32 tilewidth;
    std::string orientation;
    std::string renderorder;
    std::string tiledversion;
    std::string type;
    std::string version;
    std::vector<tileset_t> tilesets;
    std::vector<std::variant<chunk_layer_t, object_layer_t>> layers;
};

void load_tmj(WorldLoader::Opts &opts, tmj_t &tmj) {
    std::string buffer;
    buffer.reserve(128);

    std::string path;

    for(auto dir : opts.m_resources_dirs) {
        if(fs::exists(fs::path(dir) / opts.filename)) {
            path = fs::path(dir) / opts.filename;
        }
    }

    if(path.empty()) {
        logc("failed to load world: no such file {}", opts.filename);
    }

    auto ec = glz::read_file_json<glz::opts{.error_on_unknown_keys=true, .error_on_missing_keys=true}>(tmj, path, buffer);
        
    if(ec) {
        pa::logc("Failed to load config.\n{}", glz::format_error(ec, buffer));
        std::exit(1);
    }
}

void WorldLoader::load(Opts opts) {
    tmj_t tmj{};
    load_tmj(opts, tmj);

    factory::Position{registry, entity}({.vector = {0, 0}});
    factory::Drawable{registry, entity}({.visible = true});
    factory::PhysicalBody{registry, entity}({});

    /// TODO: not hacked loading
    entt::overloaded ov{
        [&tmj, &opts, this](chunk_layer_t layer) {
            for(u64 i{}; i < layer.data.size(); i++) {
                if(layer.data[i] != 1) continue;

                glm::vec2 tilesize{tmj.tilewidth, tmj.tileheight};

                glm::vec2 position{f32(i % layer.width), glm::floor(f32(i) / (layer.width))};
                position *= tilesize;
                position += glm::vec2{layer.x, layer.y};

                // logt("x:{:>10.5f} y:{:>10.5f}", position.x, position.y);

                factory::Sprite{registry, entity}({
                    .texture_id = opts.state.get_resource_manager().load_texture("test_block.png"),
                    .offset = position,
                });

                factory::PhysicalCollider{registry, entity}({
                    .box = {position, tilesize},
                    .mask = physics::create_mask({physics::Layer::StaticEnviroment, physics::Layer::Player}),
                });
            }
        },
        [this, &opts](object_layer_t layer) {
            glm::vec2 player_position{};
            glm::vec2 finish_position{};
            for(auto &obj : layer.objects) {
                if (obj.name == "Player") {
                    player_position.x = layer.x + obj.x;
                    player_position.y = layer.y + obj.y;
                } else if (obj.name == "Finish") {
                    finish_position.x = layer.x + obj.x;
                    finish_position.y = layer.y + obj.y;
                }
            }
            factory::Sprite{registry, entity}({
                .texture_id = opts.state.get_resource_manager().load_texture("go.png"),
                .offset = player_position,
                .origin = {0.5, 0.5},
            });
            factory::Sprite{registry, entity}({
                .texture_id = opts.state.get_resource_manager().load_texture("finish.png"),
                .offset = finish_position,
                .origin = {0.5, 0.5},
            });
            registry.emplace_or_replace<comp::World>(entity, comp::World{ .player_position = player_position, .finish_position = finish_position });
        },
    };

    for(auto layer : tmj.layers) {
        std::visit(ov, layer);
    }
}

}