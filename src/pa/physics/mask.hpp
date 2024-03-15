#pragma once

#include <pa/common.hpp>
#include <pa/physics/layer.hpp>
#include <bitset>

namespace pa::physics {

using Mask = std::bitset<256>;

inline Mask create_mask(const std::initializer_list<Layer> &layers) {
    Mask m;
    for(auto layer : layers) {
        m[u8(layer)] = true;
    }
    return m;
}

}