#pragma once

#include <pa/num_types.hpp>
#include <pa/concepts.hpp>
#include <komihash.h>
#include <string>

namespace pa {
namespace detail::komihash {
[[nodiscard]] inline u64 hash(const void *const msg, u64 msgLen, const u64 seed = 0) {
    return ::komihash(msg, msgLen, seed);
}
}

template<typename T>
struct hash;

template<pa::object_with_unique_representations T>
struct hash<T> {
    [[nodiscard]] u64 operator()(const T num, u64 seed = 0) const noexcept {
        return detail::komihash::hash(&num, sizeof(num), seed);
    }
};

template<typename CharT>
struct hash<std::basic_string<CharT>> {
    [[nodiscard]] u64 operator()(const std::basic_string<CharT> str, u64 seed = 0) const noexcept {
        return detail::komihash::hash(str.data(), sizeof(CharT) * str.size(), seed);
    }
};

template<typename CharT>
struct hash<std::basic_string_view<CharT>>  {
    [[nodiscard]] u64 operator()(const std::basic_string<CharT> str, u64 seed = 0) const noexcept {
        return detail::komihash::hash(str.data(), sizeof(CharT) * str.size(), seed);
    }
};

template<pa::enumeration T>
struct hash<T> {
    [[nodiscard]] u64 operator()(const T enm, u64 seed = 0) const noexcept {
        using underlying = std::underlying_type<T>;
        return detail::komihash::hash(reinterpret_cast<underlying>(enm), sizeof(underlying), seed);
    }
};
}