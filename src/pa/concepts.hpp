#pragma once

#include <type_traits>

namespace pa {
template<typename T>
concept enumeration = std::is_enum_v<T>;

template<typename T>
concept arithmetic = std::is_arithmetic_v<T>;

template<typename T>
concept object_with_unique_representations = std::has_unique_object_representations_v<T>;
}