#pragma once

#include "Engine/Core/Base.h"
#include "Engine/Core/TimeStep.h"
#include <glm/glm.hpp>

namespace Engine {

    class Scene;

    struct RaycastHit2D {
        bool Hit = false;
        glm::vec2 Point = { 0.0f, 0.0f };
        glm::vec2 Normal = { 0.0f, 0.0f };
        float Distance = 0.0f;
        void* Body = nullptr;
    };

    class Physics2D {
    public:
        static void Init();
        static void Shutdown();
        
        static void SetGravity(const glm::vec2& gravity);
        static glm::vec2 GetGravity();
        
        // Raycasting
        static RaycastHit2D Raycast(const glm::vec2& origin, const glm::vec2& direction, float maxDistance = 100.0f);
        
        // Physics step
        static void Step(float timestep, int32_t velocityIterations = 8, int32_t positionIterations = 3);
        
        // Debug rendering
        static void SetDebugDraw(bool enabled);
        static bool IsDebugDrawEnabled();
        
    private:
        static void* s_PhysicsWorld;
        static bool s_DebugDraw;
    };

}

