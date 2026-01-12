#pragma once

#include "Engine/Core/Base.h"
#include <glm/glm.hpp>

namespace Engine {

    // Rigidbody2D Component
    struct Rigidbody2DComponent {
        enum class BodyType { Static = 0, Dynamic = 1, Kinematic = 2 };
        
        BodyType Type = BodyType::Dynamic;
        bool FixedRotation = false;
        
        // Physics properties
        float Mass = 1.0f;
        float LinearDrag = 0.0f;
        float AngularDrag = 0.01f;
        float GravityScale = 1.0f;
        
        // Velocity (can be set/read)
        glm::vec2 Velocity = { 0.0f, 0.0f };
        float AngularVelocity = 0.0f;
        
        // Runtime data (not serialized)
        void* RuntimeBody = nullptr;
        
        Rigidbody2DComponent() = default;
        Rigidbody2DComponent(BodyType type) : Type(type) {}
        
        // Apply force
        void ApplyForce(const glm::vec2& force, const glm::vec2& point, bool wake = true);
        void ApplyForceToCenter(const glm::vec2& force, bool wake = true);
        void ApplyLinearImpulse(const glm::vec2& impulse, const glm::vec2& point, bool wake = true);
        void ApplyLinearImpulseToCenter(const glm::vec2& impulse, bool wake = true);
        void ApplyAngularImpulse(float impulse, bool wake = true);
    };

    // Box Collider Component
    struct BoxCollider2DComponent {
        glm::vec2 Offset = { 0.0f, 0.0f };
        glm::vec2 Size = { 0.5f, 0.5f };
        
        // Physics material
        float Density = 1.0f;
        float Friction = 0.5f;
        float Restitution = 0.0f;        // Bounciness (0 = no bounce, 1 = perfect bounce)
        float RestitutionThreshold = 0.5f;
        
        // Trigger
        bool IsSensor = false;
        
        // Runtime data (not serialized)
        void* RuntimeFixture = nullptr;
        
        BoxCollider2DComponent() = default;
    };

    // Circle Collider Component
    struct CircleCollider2DComponent {
        glm::vec2 Offset = { 0.0f, 0.0f };
        float Radius = 0.5f;
        
        // Physics material
        float Density = 1.0f;
        float Friction = 0.5f;
        float Restitution = 0.0f;
        float RestitutionThreshold = 0.5f;
        
        // Trigger
        bool IsSensor = false;
        
        // Runtime data (not serialized)
        void* RuntimeFixture = nullptr;
        
        CircleCollider2DComponent() = default;
    };

}

