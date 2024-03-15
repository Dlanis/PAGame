#pragma once

#include <pa/common.hpp>

namespace pa::core {

class ID {
    u64 m_number{};

public:
    ID();
    ~ID() = default;

    friend inline bool operator==(ID lhs, ID rhs) {
        return lhs.m_number == rhs.m_number;
    }
};

}