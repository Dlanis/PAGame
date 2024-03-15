#include "id.hpp"

#include <atomic>

std::atomic_uint64_t g_pa_core_id_counter{0};

namespace pa::core {

ID::ID() {
    m_number = ++g_pa_core_id_counter;
}

}