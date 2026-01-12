#pragma once

#include "Engine/Core/Base.h"
#include <glm/glm.hpp>

namespace Engine {

    class Shader {
    public:
        Shader(const std::string& filepath);
        Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
        ~Shader();
        
        void Bind() const;
        void Unbind() const;
        
        // Set uniforms
        void SetInt(const std::string& name, int value);
        void SetIntArray(const std::string& name, int* values, uint32_t count);
        void SetFloat(const std::string& name, float value);
        void SetFloat2(const std::string& name, const glm::vec2& value);
        void SetFloat3(const std::string& name, const glm::vec3& value);
        void SetFloat4(const std::string& name, const glm::vec4& value);
        void SetMat3(const std::string& name, const glm::mat3& value);
        void SetMat4(const std::string& name, const glm::mat4& value);
        
        const std::string& GetName() const { return m_Name; }
        
        static Ref<Shader> Create(const std::string& filepath);
        static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
        
    private:
        std::string ReadFile(const std::string& filepath);
        std::unordered_map<uint32_t, std::string> PreProcess(const std::string& source);
        void Compile(const std::unordered_map<uint32_t, std::string>& shaderSources);
        
        int GetUniformLocation(const std::string& name);
        
    private:
        uint32_t m_RendererID;
        std::string m_Name;
        std::unordered_map<std::string, int> m_UniformLocationCache;
    };

}
