#include "Engine/Renderer/ParticleSystem.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Core/Logger.h"
#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

namespace Engine {

    ParticleSystem::ParticleSystem(uint32_t maxParticles)
        : m_MaxParticles(maxParticles)
    {
        m_ParticlePool.resize(maxParticles);
    }

    void ParticleSystem::Emit(const ParticleProps& props)
    {
        Particle& particle = m_ParticlePool[FindUnusedParticle()];

        particle.Active = true;
        particle.Position = props.Position;
        particle.Rotation = props.Rotation;
        particle.AngularVelocity = props.AngularVelocity;

        // Velocity
        particle.Velocity = props.Velocity;
        particle.Velocity.x += props.VelocityVariation.x * ((rand() % 200 - 100) / 100.0f);
        particle.Velocity.y += props.VelocityVariation.y * ((rand() % 200 - 100) / 100.0f);

        // Color
        particle.ColorBegin = props.ColorBegin;
        particle.ColorEnd = props.ColorEnd;

        // Size
        particle.SizeBegin = props.SizeBegin + props.SizeVariation * ((rand() % 200 - 100) / 100.0f);
        particle.SizeEnd = props.SizeEnd;

        // Life
        particle.LifeTime = props.LifeTime;
        particle.LifeRemaining = props.LifeTime;
    }

    void ParticleSystem::EmitBurst(const ParticleProps& props, uint32_t count)
    {
        for (uint32_t i = 0; i < count; ++i)
        {
            Emit(props);
        }
    }

    void ParticleSystem::OnUpdate(float deltaTime)
    {
        for (auto& particle : m_ParticlePool)
        {
            if (!particle.Active)
                continue;

            if (particle.LifeRemaining <= 0.0f)
            {
                particle.Active = false;
                continue;
            }

            particle.LifeRemaining -= deltaTime;
            particle.Position += particle.Velocity * deltaTime;
            particle.Rotation += particle.AngularVelocity * deltaTime;
        }
    }

    void ParticleSystem::OnRender()
    {
        for (auto& particle : m_ParticlePool)
        {
            if (!particle.Active)
                continue;

            // Fade away particles
            float life = particle.LifeRemaining / particle.LifeTime;
            glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
            color.a = color.a * life;

            float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

            // Render particle
            Renderer2D::DrawRotatedQuad(
                { particle.Position.x, particle.Position.y, 0.0f },
                { size, size },
                particle.Rotation,
                color
            );
        }
    }

    void ParticleSystem::Clear()
    {
        for (auto& particle : m_ParticlePool)
        {
            particle.Active = false;
        }
        m_PoolIndex = 0;
    }

    uint32_t ParticleSystem::GetActiveParticleCount() const
    {
        uint32_t count = 0;
        for (const auto& particle : m_ParticlePool)
        {
            if (particle.Active)
                ++count;
        }
        return count;
    }

    uint32_t ParticleSystem::FindUnusedParticle()
    {
        // Start from last known position for better cache coherence
        for (uint32_t i = m_PoolIndex; i < m_MaxParticles; ++i)
        {
            if (!m_ParticlePool[i].Active)
            {
                m_PoolIndex = i;
                return i;
            }
        }

        // Search from beginning
        for (uint32_t i = 0; i < m_PoolIndex; ++i)
        {
            if (!m_ParticlePool[i].Active)
            {
                m_PoolIndex = i;
                return i;
            }
        }

        // Override oldest particle if pool is full
        m_PoolIndex = (m_PoolIndex + 1) % m_MaxParticles;
        return m_PoolIndex;
    }

} // namespace Engine

