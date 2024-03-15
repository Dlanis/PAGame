#pragma once

#include <pa/common.hpp>
#include <vector>
#include <pa/drawing/sprite.hpp>

namespace pa::comp {

class Sprites final {
    using T = std::vector<drawing::Sprite>;
    T m_container;

public:
    T& operator()() {
        return m_container;
    }
};

}