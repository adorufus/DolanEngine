#include "Engine/Renderer/CameraController.h"
#include "Engine/Core/Logger.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>
#include <cstdlib>
#include <cmath>

namespace Engine {

    CameraController::CameraController(OrthographicCamera* camera)
        : m_Camera(camera)
    {
    }

    void CameraController::Update(float deltaTime)
    {
        if (!m_Camera)
            return;

        // Apply follow behavior
        if (m_FollowTarget)
        {
            ApplyFollow(deltaTime);
        }

        // Apply shake effect
        if (m_ShakeTimeRemaining > 0.0f)
        {
            ApplyShake(deltaTime);
        }

        // Apply bounds constraints
        if (m_UseBounds)
        {
            ApplyBounds();
        }
    }

    void CameraController::ApplyFollow(float deltaTime)
    {
        glm::vec3 currentPos = m_Camera->GetPosition();
        glm::vec3 targetPos = *m_FollowTarget;

        switch (m_FollowMode)
        {
            case CameraFollowMode::Instant:
            {
                currentPos.x = targetPos.x;
                currentPos.y = targetPos.y;
                break;
            }

            case CameraFollowMode::Smooth:
            {
                float t = 1.0f - std::pow(0.5f, deltaTime * m_FollowSpeed);
                currentPos.x = glm::lerp(currentPos.x, targetPos.x, t);
                currentPos.y = glm::lerp(currentPos.y, targetPos.y, t);
                break;
            }

            case CameraFollowMode::Deadzone:
            {
                float dx = targetPos.x - currentPos.x;
                float dy = targetPos.y - currentPos.y;

                // Only move if target is outside deadzone
                if (std::abs(dx) > m_Deadzone.x)
                {
                    float moveX = dx - (dx > 0 ? m_Deadzone.x : -m_Deadzone.x);
                    currentPos.x += moveX * m_FollowSpeed * deltaTime;
                }

                if (std::abs(dy) > m_Deadzone.y)
                {
                    float moveY = dy - (dy > 0 ? m_Deadzone.y : -m_Deadzone.y);
                    currentPos.y += moveY * m_FollowSpeed * deltaTime;
                }
                break;
            }
        }

        m_Camera->SetPosition(currentPos);
    }

    void CameraController::ApplyShake(float deltaTime)
    {
        m_ShakeTimeRemaining -= deltaTime;

        if (m_ShakeTimeRemaining <= 0.0f)
        {
            // Shake finished, reset offset
            m_ShakeTimeRemaining = 0.0f;
            m_ShakeOffset = { 0.0f, 0.0f };
            return;
        }

        // Calculate shake intensity falloff
        float shakeAmount = m_ShakeIntensity * (m_ShakeTimeRemaining / m_ShakeDuration);

        // Generate new random offset
        glm::vec2 newOffset = GetRandomOffset() * shakeAmount;

        // Remove previous shake offset from camera position
        glm::vec3 pos = m_Camera->GetPosition();
        pos.x -= m_ShakeOffset.x;
        pos.y -= m_ShakeOffset.y;

        // Apply new shake offset
        pos.x += newOffset.x;
        pos.y += newOffset.y;
        m_Camera->SetPosition(pos);

        m_ShakeOffset = newOffset;
    }

    void CameraController::ApplyBounds()
    {
        glm::vec3 pos = m_Camera->GetPosition();

        // Clamp position within bounds
        pos.x = glm::clamp(pos.x, m_BoundsMin.x, m_BoundsMax.x);
        pos.y = glm::clamp(pos.y, m_BoundsMin.y, m_BoundsMax.y);

        m_Camera->SetPosition(pos);
    }

    glm::vec2 CameraController::GetRandomOffset() const
    {
        float x = ((rand() % 200) - 100) / 100.0f;
        float y = ((rand() % 200) - 100) / 100.0f;
        return { x, y };
    }

    void CameraController::Shake(float intensity, float duration)
    {
        m_ShakeIntensity = intensity;
        m_ShakeDuration = duration;
        m_ShakeTimeRemaining = duration;
        GE_CORE_TRACE("Camera shake started: intensity={0}, duration={1}", intensity, duration);
    }

    void CameraController::StopShake()
    {
        // Remove current shake offset
        if (m_ShakeTimeRemaining > 0.0f)
        {
            glm::vec3 pos = m_Camera->GetPosition();
            pos.x -= m_ShakeOffset.x;
            pos.y -= m_ShakeOffset.y;
            m_Camera->SetPosition(pos);
        }

        m_ShakeTimeRemaining = 0.0f;
        m_ShakeOffset = { 0.0f, 0.0f };
    }

    void CameraController::ZoomIn(float amount)
    {
        if (!m_Camera)
            return;
            
        float currentZoom = m_Camera->GetZoomLevel();
        float newZoom = currentZoom + (amount * m_ZoomSpeed);
        m_Camera->SetZoomLevel(newZoom);
        GE_CORE_TRACE("Camera zoom in: {0}", newZoom);
    }

    void CameraController::ZoomOut(float amount)
    {
        if (!m_Camera)
            return;
            
        float currentZoom = m_Camera->GetZoomLevel();
        float newZoom = currentZoom - (amount * m_ZoomSpeed);
        m_Camera->SetZoomLevel(newZoom);
        GE_CORE_TRACE("Camera zoom out: {0}", newZoom);
    }

    void CameraController::SetBounds(const glm::vec2& min, const glm::vec2& max)
    {
        m_BoundsMin = min;
        m_BoundsMax = max;
        m_UseBounds = true;
        GE_CORE_TRACE("Camera bounds set: min=({0}, {1}), max=({2}, {3})", 
                      min.x, min.y, max.x, max.y);
    }

} // namespace Engine

