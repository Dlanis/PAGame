#pragma once

#include <pa/common.hpp>

namespace pa::comp {

struct Camera final {
    glm::vec2 offset;  /// Camera offset (displacement from target)
    glm::vec2 target;  /// Camera target (rotation and zoom origin)
    f32 rotation;      /// Camera rotation in degrees
    f32 zoom;          /// Camera zoom (scaling), should be 1.0f by default

    inline f64 get_current_time() {
        return set_current_time;
    }

    inline void make_current() {
        set_current_time = ::GetTime();
    }

private:
    f64 set_current_time;  /// Using time to track if it current
};

}