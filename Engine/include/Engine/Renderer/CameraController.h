#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Renderer/OrthographicCamera.h"
#include <glm/glm.hpp>

namespace Engine {

    // Camera follow modes
    enum class CameraFollowMode {
        Instant,       // Snap to target immediately
        Smooth,        // Smooth lerp to target
        Deadzone       // Only move when outside deadzone
    };

    // Camera controller with follow and effects
    class CameraController {
    public:
        CameraController() = default;
        CameraController(OrthographicCamera* camera);

        void SetCamera(OrthographicCamera* camera) { m_Camera = camera; }
        OrthographicCamera* GetCamera() { return m_Camera; }

        // Update camera (apply follow, shake, etc.)
        void Update(float deltaTime);

        // Camera follow
        void SetTarget(const glm::vec3* target) { m_FollowTarget = target; }
        void SetFollowMode(CameraFollowMode mode) { m_FollowMode = mode; }
        void SetFollowSpeed(float speed) { m_FollowSpeed = speed; }
        void SetDeadzone(const glm::vec2& deadzone) { m_Deadzone = deadzone; }
        
        bool IsFollowing() const { return m_FollowTarget != nullptr; }

        // Camera shake
        void Shake(float intensity, float duration);
        void StopShake();
        bool IsShaking() const { return m_ShakeTimeRemaining > 0.0f; }

        // Camera zoom
        void SetZoomSpeed(float speed) { m_ZoomSpeed = speed; }
        void ZoomIn(float amount);
        void ZoomOut(float amount);

        // Bounds (optional camera constraints)
        void SetBounds(const glm::vec2& min, const glm::vec2& max);
        void ClearBounds() { m_UseBounds = false; }

    private:
        void ApplyFollow(float deltaTime);
        void ApplyShake(float deltaTime);
        void ApplyBounds();
        glm::vec2 GetRandomOffset() const;

    private:
        OrthographicCamera* m_Camera = nullptr;
        
        // Follow
        const glm::vec3* m_FollowTarget = nullptr;
        CameraFollowMode m_FollowMode = CameraFollowMode::Smooth;
        float m_FollowSpeed = 5.0f;
        glm::vec2 m_Deadzone = { 2.0f, 2.0f };
        
        // Shake
        float m_ShakeIntensity = 0.0f;
        float m_ShakeDuration = 0.0f;
        float m_ShakeTimeRemaining = 0.0f;
        glm::vec2 m_ShakeOffset = { 0.0f, 0.0f };
        
        // Zoom
        float m_ZoomSpeed = 1.0f;
        
        // Bounds
        bool m_UseBounds = false;
        glm::vec2 m_BoundsMin = { 0.0f, 0.0f };
        glm::vec2 m_BoundsMax = { 0.0f, 0.0f };
    };

} // namespace Engine

