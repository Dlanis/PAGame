#pragma once

#include <pa/resource_id/texture_id.hpp>
#include <pa/common.hpp>
#include <pa/hash.hpp>
#include <ankerl/unordered_dense.h>
#include <pa/raylib/texture2d_deleter.hpp>

namespace pa::core {

class Config;

class ResourceManager final {
    std::vector<std::string> m_resources_dirs;
    ankerl::unordered_dense::map<resource_id::TextureID, std::shared_ptr<::Texture>, pa::hash<u64>> m_texture_cache;

    bool m_inited{false};

public:
    ~ResourceManager();

    void init(const Config& config);
    void terminate();

    std::optional<std::shared_ptr<::Texture>> get_texture(resource_id::TextureID texture_id);
    std::optional<resource_id::TextureID> load_texture(std::string_view filename);

};

}
