#pragma once

#include <pa/common.hpp>
#include <pa/core/id.hpp>
#include <vector>
#include <utility>

namespace pa::comp {

class AreaResponse final {
    using T = std::vector<std::pair<core::ID, bool>>;
    T m_container;

public:
    T& operator()() {
        return m_container;
    }
};

}