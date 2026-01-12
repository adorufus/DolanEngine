#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/SubTexture2D.h"
#include "Engine/Renderer/OrthographicCamera.h"
#include "Engine/Physics/PhysicsComponents.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

    struct CameraComponent {
        OrthographicCamera Camera;
        bool Primary = true; // Main camera in scene
        bool FixedAspectRatio = false;
        
        CameraComponent() = default;
        CameraComponent(const OrthographicCamera& camera)
            : Camera(camera) {}
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
        std::string ScriptName;
        
        // TODO: Scripting system in future phase
        
        ScriptComponent() = default;
        ScriptComponent(const std::string& scriptName)
            : ScriptName(scriptName) {}
    };

}

