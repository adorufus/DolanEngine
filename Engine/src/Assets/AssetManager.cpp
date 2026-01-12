#include "Engine/Assets/AssetManager.h"
#include "Engine/Core/Logger.h"

namespace Engine {

    std::unordered_map<std::string, Ref<Texture2D>> AssetManager::s_Textures;
    std::unordered_map<std::string, Ref<Shader>> AssetManager::s_Shaders;

    Ref<Texture2D> AssetManager::LoadTexture(const std::string& name, const std::string& path) {
        if (s_Textures.find(name) != s_Textures.end()) {
            GE_CORE_WARN("Texture '{0}' already loaded, returning cached version", name);
            return s_Textures[name];
        }
        
        auto texture = Texture2D::Create(path);
        if (texture->IsLoaded()) {
            s_Textures[name] = texture;
            GE_CORE_INFO("Loaded texture '{0}' from '{1}'", name, path);
            return texture;
        }
        
        GE_CORE_ERROR("Failed to load texture '{0}' from '{1}'", name, path);
        return nullptr;
    }

    Ref<Texture2D> AssetManager::GetTexture(const std::string& name) {
        if (s_Textures.find(name) == s_Textures.end()) {
            GE_CORE_ERROR("Texture '{0}' not found in asset manager", name);
            return nullptr;
        }
        return s_Textures[name];
    }

    bool AssetManager::HasTexture(const std::string& name) {
        return s_Textures.find(name) != s_Textures.end();
    }

    void AssetManager::UnloadTexture(const std::string& name) {
        if (s_Textures.find(name) != s_Textures.end()) {
            s_Textures.erase(name);
            GE_CORE_INFO("Unloaded texture '{0}'", name);
        }
    }

    Ref<Shader> AssetManager::LoadShader(const std::string& name, const std::string& path) {
        if (s_Shaders.find(name) != s_Shaders.end()) {
            GE_CORE_WARN("Shader '{0}' already loaded, returning cached version", name);
            return s_Shaders[name];
        }
        
        auto shader = Shader::Create(path);
        s_Shaders[name] = shader;
        GE_CORE_INFO("Loaded shader '{0}' from '{1}'", name, path);
        return shader;
    }

    Ref<Shader> AssetManager::GetShader(const std::string& name) {
        if (s_Shaders.find(name) == s_Shaders.end()) {
            GE_CORE_ERROR("Shader '{0}' not found in asset manager", name);
            return nullptr;
        }
        return s_Shaders[name];
    }

    bool AssetManager::HasShader(const std::string& name) {
        return s_Shaders.find(name) != s_Shaders.end();
    }

    void AssetManager::UnloadShader(const std::string& name) {
        if (s_Shaders.find(name) != s_Shaders.end()) {
            s_Shaders.erase(name);
            GE_CORE_INFO("Unloaded shader '{0}'", name);
        }
    }

    void AssetManager::Clear() {
        s_Textures.clear();
        s_Shaders.clear();
        GE_CORE_INFO("Cleared all assets from AssetManager");
    }

    AssetManager::Stats AssetManager::GetStats() {
        Stats stats;
        stats.TexturesLoaded = (uint32_t)s_Textures.size();
        stats.ShadersLoaded = (uint32_t)s_Shaders.size();
        
        // Estimate memory usage
        for (const auto& [name, texture] : s_Textures) {
            stats.EstimatedMemoryUsage += texture->GetWidth() * texture->GetHeight() * 4; // RGBA
        }
        
        return stats;
    }

}

