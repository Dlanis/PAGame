#include "resource_manager.hpp"

#include <pa/core/config.hpp>

namespace pa::core {

ResourceManager::~ResourceManager() {
    if(m_inited) {
        loge("destructed before terminalization.");
    }
}

void ResourceManager::init(const Config& config) {
    m_resources_dirs = config.get_resources_dirs();
    m_inited = true;
}

void ResourceManager::terminate() {
    for(auto& entry : m_texture_cache) {
        ::UnloadTexture(*entry.second.get());
    }
    m_inited = false;
}

std::optional<std::shared_ptr<Texture>> ResourceManager::get_texture(resource_id::TextureID texture_id) {
    if(m_texture_cache.contains(texture_id)) {
        return m_texture_cache[texture_id];
    }

    return std::nullopt;
}

std::optional<resource_id::TextureID> ResourceManager::load_texture(std::string_view filename) {
    if(!m_inited) {
        loge("use of not inited object");
        return std::nullopt;
    }

    fs::path normalized_filename{fs::path(filename).lexically_normal()};

    for(auto &resources_dir : m_resources_dirs) {
        fs::path full_path{resources_dir / normalized_filename};
        
        resource_id::TextureID texture_id{pa::hash<std::string>{}(full_path)};

        if(m_texture_cache.contains(texture_id)) {
            return texture_id;
        }

        if(fs::exists(full_path)) {
            ::Texture texture{::LoadTexture(full_path.c_str())};

            if(texture.id <= 0) {
                logd("failed to load texture '{}'", full_path.string());
                continue;
            }

            m_texture_cache[texture_id] = std::make_shared<::Texture>(texture);
            return texture_id;
        }
    }

    logd("no texture found '{}'", normalized_filename.string());

    return std::nullopt;
}

}