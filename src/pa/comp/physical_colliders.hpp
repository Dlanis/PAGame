#pragma once

#include <pa/common.hpp>
#include <vector>
#include <pa/physics/collider.hpp>

namespace pa::comp {

class PhysicalColliders final {
    using T = std::vector<physics::Collider>;
    T m_container;

public:
    T& operator()() {
        return m_container;
    }
};

}