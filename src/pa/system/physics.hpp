#pragma once

#include <pa/common.hpp>

namespace pa::core {
    class Config;
    class State;
}

namespace pa::system {

class Physics final {
public:
    void update(core::State& state, const core::Config &config, const f64 delta_time);
};

}