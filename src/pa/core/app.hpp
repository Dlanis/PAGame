#pragma once

#include <pa/common.hpp>

namespace pa::core {

class App final {
   public:
    explicit App() = default;
    ~App() = default;

    void run();
};

}