#include "Engine/Physics/Physics2D.h"
#include "Engine/Core/Logger.h"
#include <box2d/box2d.h>

namespace Engine {

    void* Physics2D::s_PhysicsWorld = nullptr;
    bool Physics2D::s_DebugDraw = false;

    void Physics2D::Init() {
        // Physics world will be created per-scene
        GE_CORE_INFO("Physics2D initialized");
    }

    void Physics2D::Shutdown() {
        GE_CORE_INFO("Physics2D shutdown");
    }

    void Physics2D::SetGravity(const glm::vec2& gravity) {
        if (s_PhysicsWorld) {
            b2World* world = static_cast<b2World*>(s_PhysicsWorld);
            world->SetGravity(b2Vec2(gravity.x, gravity.y));
        }
    }

    glm::vec2 Physics2D::GetGravity() {
        if (s_PhysicsWorld) {
            b2World* world = static_cast<b2World*>(s_PhysicsWorld);
            b2Vec2 gravity = world->GetGravity();
            return { gravity.x, gravity.y };
        }
        return { 0.0f, -9.81f };
    }

    RaycastHit2D Physics2D::Raycast(const glm::vec2& origin, const glm::vec2& direction, float maxDistance) {
        RaycastHit2D hit;
        
        if (!s_PhysicsWorld)
            return hit;
        
        b2World* world = static_cast<b2World*>(s_PhysicsWorld);
        
        glm::vec2 end = origin + direction * maxDistance;
        
        class RaycastCallback : public b2RayCastCallback {
        public:
            RaycastHit2D& Hit;
            
            RaycastCallback(RaycastHit2D& hit) : Hit(hit) {}
            
            float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override {
                Hit.Hit = true;
                Hit.Point = { point.x, point.y };
                Hit.Normal = { normal.x, normal.y };
                Hit.Distance = fraction;
                Hit.Body = fixture->GetBody();
                return fraction; // Return fraction to find closest hit
            }
        };
        
        RaycastCallback callback(hit);
        world->RayCast(&callback, b2Vec2(origin.x, origin.y), b2Vec2(end.x, end.y));
        
        return hit;
    }

    void Physics2D::Step(float timestep, int32_t velocityIterations, int32_t positionIterations) {
        if (s_PhysicsWorld) {
            b2World* world = static_cast<b2World*>(s_PhysicsWorld);
            world->Step(timestep, velocityIterations, positionIterations);
        }
    }

    void Physics2D::SetDebugDraw(bool enabled) {
        s_DebugDraw = enabled;
    }

    bool Physics2D::IsDebugDrawEnabled() {
        return s_DebugDraw;
    }

}

