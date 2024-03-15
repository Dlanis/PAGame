#pragma once

#include "pa/core/state.hpp"
#include <pa/common.hpp>

namespace pa::core {

class WorldLoader final {
    entt::registry &registry;
    entt::entity &entity;

public:
    WorldLoader(entt::registry &registry, entt::entity &entity) : registry(registry), entity(entity) {}

    struct Opts {
        core::State &state;
        std::vector<std::string> m_resources_dirs;
        std::string_view filename;
    };

    void load(Opts opts);
};

}