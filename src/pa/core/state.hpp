#pragma once

#include <pa/core/resource_manager.hpp>
#include <pa/simple_gettersetter.hpp>
#include <pa/common.hpp>

namespace pa::core {

class State final {
    entt::registry m_registry;
    entt::dispatcher m_dispatcher;
    ResourceManager m_resource_manager;

public:

    entt::registry& get_registry();
    entt::dispatcher& get_dispatcher();
    ResourceManager& get_resource_manager();
};

}