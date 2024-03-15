#pragma once

#include <pa/common.hpp>

namespace pa::core {
    class Config;
    class State;
}

namespace pa::system {

class Render final {
    bool m_inited{false};

   public:
    ~Render();

    void init(const core::Config& config);
    void terminate();
    void update(core::State& state, const core::Config &config, const f64 delta_time);
};

}