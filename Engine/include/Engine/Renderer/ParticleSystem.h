#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Renderer/Texture.h"
#include <glm/glm.hpp>
#include <vector>

namespace Engine {

    // Single particle
    struct Particle {
        glm::vec2 Position;
        glm::vec2 Velocity;
        glm::vec4 ColorBegin, ColorEnd;
        float Rotation = 0.0f;
        float AngularVelocity = 0.0f;
        float SizeBegin, SizeEnd;
        float LifeTime = 1.0f;
        float LifeRemaining = 0.0f;
        bool Active = false;
    };

    // Particle properties for spawning
    struct ParticleProps {
        glm::vec2 Position;
        glm::vec2 Velocity, VelocityVariation;
        glm::vec4 ColorBegin, ColorEnd;
        float SizeBegin = 0.5f, SizeEnd = 0.0f;
        float SizeVariation = 0.3f;
        float LifeTime = 1.0f;
        float Rotation = 0.0f;
        float AngularVelocity = 0.0f;
        Ref<Texture2D> Texture;
    };

    // Particle system for rendering and updating particles
    class ParticleSystem {
    public:
        ParticleSystem(uint32_t maxParticles = 10000);
        ~ParticleSystem() = default;

        // Emit a single particle
        void Emit(const ParticleProps& props);
        
        // Emit multiple particles
        void EmitBurst(const ParticleProps& props, uint32_t count);
        
        // Update all active particles
        void OnUpdate(float deltaTime);
        
        // Render all active particles
        void OnRender();
        
        // Clear all particles
        void Clear();
        
        // Get statistics
        uint32_t GetActiveParticleCount() const;
        uint32_t GetMaxParticles() const { return m_MaxParticles; }

    private:
        uint32_t FindUnusedParticle();

    private:
        std::vector<Particle> m_ParticlePool;
        uint32_t m_MaxParticles;
        uint32_t m_PoolIndex = 0;
    };

} // namespace Engine

