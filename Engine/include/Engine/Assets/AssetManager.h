#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/Shader.h"

namespace Engine {

    class AssetManager {
    public:
        // Textures
        static Ref<Texture2D> LoadTexture(const std::string& name, const std::string& path);
        static Ref<Texture2D> GetTexture(const std::string& name);
        static bool HasTexture(const std::string& name);
        static void UnloadTexture(const std::string& name);
        
        // Shaders
        static Ref<Shader> LoadShader(const std::string& name, const std::string& path);
        static Ref<Shader> GetShader(const std::string& name);
        static bool HasShader(const std::string& name);
        static void UnloadShader(const std::string& name);
        
        // Clear all assets
        static void Clear();
        
        // Statistics
        struct Stats {
            uint32_t TexturesLoaded = 0;
            uint32_t ShadersLoaded = 0;
            uint64_t EstimatedMemoryUsage = 0;
        };
        static Stats GetStats();
        
    private:
        static std::unordered_map<std::string, Ref<Texture2D>> s_Textures;
        static std::unordered_map<std::string, Ref<Shader>> s_Shaders;
    };

}

