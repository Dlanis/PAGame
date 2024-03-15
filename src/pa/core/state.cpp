#include "state.hpp"

namespace pa::core {

entt::registry& State::get_registry() {
    return m_registry;    
}

entt::dispatcher& State::get_dispatcher() {
    return m_dispatcher;
}

ResourceManager& State::get_resource_manager() {
    return m_resource_manager;
}


}