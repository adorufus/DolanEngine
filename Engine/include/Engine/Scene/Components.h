#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/SubTexture2D.h"
#include "Engine/Renderer/OrthographicCamera.h"
#include "Engine/Renderer/ParticleSystem.h"
#include "Engine/Physics/PhysicsComponents.h"
#include "Engine/Animation/Animator.h"
#include "Engine/Audio/AudioSource.h"
#include "Engine/Audio/AudioBuffer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sol/sol.hpp>

namespace Engine {

    struct TagComponent {
        std::string Tag;
        
        TagComponent() = default;
        TagComponent(const std::string& tag)
            : Tag(tag) {}
    };

    struct TransformComponent {
        glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
        
        TransformComponent() = default;
        TransformComponent(const glm::vec3& position)
            : Position(position) {}
        
        glm::mat4 GetTransform() const {
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1, 0, 0 })
                * glm::rotate(glm::mat4(1.0f), Rotation.y, { 0, 1, 0 })
                * glm::rotate(glm::mat4(1.0f), Rotation.z, { 0, 0, 1 });
            
            return glm::translate(glm::mat4(1.0f), Position)
                * rotation
                * glm::scale(glm::mat4(1.0f), Scale);
        }
    };

    struct SpriteRendererComponent {
        glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        Ref<Texture2D> Texture;
        Ref<SubTexture2D> SubTexture;
        float TilingFactor = 1.0f;
        
        SpriteRendererComponent() = default;
        SpriteRendererComponent(const glm::vec4& color)
            : Color(color) {}
    };

    struct AnimationComponent {
        Animator Animator;
        std::string StartAnimation;
        bool PlayOnStart = false;
        
        AnimationComponent() = default;
        AnimationComponent(const std::string& startAnim, bool playOnStart = true)
            : StartAnimation(startAnim), PlayOnStart(playOnStart) {}
    };

    struct ParticleEmitterComponent {
        ParticleProps Properties;
        float EmissionRate = 10.0f;  // Particles per second
        float EmissionTimer = 0.0f;
        bool Emit = true;
        bool BurstMode = false;
        uint32_t BurstCount = 50;
        
        ParticleEmitterComponent() {
            // Default particle properties
            Properties.ColorBegin = { 1.0f, 0.8f, 0.2f, 1.0f };
            Properties.ColorEnd = { 1.0f, 0.0f, 0.0f, 0.0f };
            Properties.SizeBegin = 0.5f;
            Properties.SizeEnd = 0.0f;
            Properties.SizeVariation = 0.3f;
            Properties.LifeTime = 1.0f;
            Properties.Velocity = { 0.0f, 2.0f };
            Properties.VelocityVariation = { 3.0f, 1.0f };
        }
    };

    struct CameraComponent {
        OrthographicCamera Camera;
        bool Primary = true; // Main camera in scene
        bool FixedAspectRatio = false;
        
        CameraComponent() = default;
        CameraComponent(const OrthographicCamera& camera)
            : Camera(camera) {}
    };

    struct CameraControllerComponent {
        // Target to follow (optional)
        void* FollowTarget = nullptr;  // Pointer to TransformComponent*
        
        // Follow settings
        int FollowMode = 1;  // 0=Instant, 1=Smooth, 2=Deadzone
        float FollowSpeed = 5.0f;
        glm::vec2 Deadzone = { 2.0f, 2.0f };
        
        // Shake
        float ShakeIntensity = 0.0f;
        float ShakeDuration = 0.0f;
        float ShakeTime = 0.0f;
        
        // Bounds
        bool UseBounds = false;
        glm::vec2 BoundsMin = { -100.0f, -100.0f };
        glm::vec2 BoundsMax = { 100.0f, 100.0f };
        
        CameraControllerComponent() = default;
    };

    // Legacy simple physics component (kept for compatibility)
    struct RigidbodyComponent {
        glm::vec2 Velocity = { 0.0f, 0.0f };
        glm::vec2 Acceleration = { 0.0f, 0.0f };
        float Mass = 1.0f;
        float Drag = 0.0f;
        bool UseGravity = false;
        
        RigidbodyComponent() = default;
    };
    
    // Note: Use Rigidbody2DComponent from PhysicsComponents.h for Box2D physics

    struct ScriptComponent {
        std::string ScriptPath;
        
        // Lua script lifecycle callbacks
        sol::protected_function OnCreate;
        sol::protected_function OnDestroy;
        sol::protected_function OnUpdate;
        
        // Instance table for this script
        sol::table Instance;
        
        ScriptComponent() = default;
        ScriptComponent(const std::string& scriptPath)
            : ScriptPath(scriptPath) {}
    };

    struct AudioSourceComponent {
        Ref<AudioSource> Source;
        Ref<AudioBuffer> Buffer;
        
        std::string FilePath;
        bool PlayOnStart = false;
        bool Is3D = true;
        bool Loop = false;
        float Volume = 1.0f;
        float Pitch = 1.0f;
        
        // 3D Audio properties
        float MinDistance = 1.0f;
        float MaxDistance = 100.0f;
        float RolloffFactor = 1.0f;
        
        AudioSourceComponent() {
            Source = AudioSource::Create();
        }
    };

    struct AudioListenerComponent {
        // Marks this entity as the audio listener
        // Position will be synced from TransformComponent
        bool Active = true;
        
        AudioListenerComponent() = default;
    };

}



