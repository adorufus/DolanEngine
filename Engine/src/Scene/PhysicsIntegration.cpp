#include "Engine/Scene/Scene.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Scene/Components.h"
#include "Engine/Core/Logger.h"
#include <box2d/box2d.h>

namespace Engine {

    static b2BodyType Rigidbody2DTypeToBox2DBody(Rigidbody2DComponent::BodyType bodyType) {
        switch (bodyType) {
            case Rigidbody2DComponent::BodyType::Static:    return b2_staticBody;
            case Rigidbody2DComponent::BodyType::Dynamic:   return b2_dynamicBody;
            case Rigidbody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
        }
        
        GE_CORE_ASSERT(false, "Unknown body type");
        return b2_staticBody;
    }

    void Scene::OnPhysics2DStart() {
        b2World* world = static_cast<b2World*>(m_PhysicsWorld);
        
        // Create Box2D bodies for all entities with Rigidbody2D component
        auto view = m_Registry.view<TransformComponent, Rigidbody2DComponent>();
        for (auto e : view) {
            Entity entity = { e, this };
            auto& transform = entity.GetComponent<TransformComponent>();
            auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
            
            b2BodyDef bodyDef;
            bodyDef.type = Rigidbody2DTypeToBox2DBody(rb2d.Type);
            bodyDef.position.Set(transform.Position.x, transform.Position.y);
            bodyDef.angle = transform.Rotation.z;
            bodyDef.fixedRotation = rb2d.FixedRotation;
            
            b2Body* body = world->CreateBody(&bodyDef);
            body->SetGravityScale(rb2d.GravityScale);
            body->SetLinearDamping(rb2d.LinearDrag);
            body->SetAngularDamping(rb2d.AngularDrag);
            
            // Store body pointer
            rb2d.RuntimeBody = body;
            
            // Create fixtures (colliders)
            if (entity.HasComponent<BoxCollider2DComponent>()) {
                auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
                
                b2PolygonShape boxShape;
                boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, 
                                  bc2d.Size.y * transform.Scale.y,
                                  b2Vec2(bc2d.Offset.x, bc2d.Offset.y), 0.0f);
                
                b2FixtureDef fixtureDef;
                fixtureDef.shape = &boxShape;
                fixtureDef.density = bc2d.Density;
                fixtureDef.friction = bc2d.Friction;
                fixtureDef.restitution = bc2d.Restitution;
                fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
                fixtureDef.isSensor = bc2d.IsSensor;
                
                b2Fixture* fixture = body->CreateFixture(&fixtureDef);
                bc2d.RuntimeFixture = fixture;
            }
            
            if (entity.HasComponent<CircleCollider2DComponent>()) {
                auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
                
                b2CircleShape circleShape;
                circleShape.m_p.Set(cc2d.Offset.x, cc2d.Offset.y);
                circleShape.m_radius = cc2d.Radius * std::max(transform.Scale.x, transform.Scale.y);
                
                b2FixtureDef fixtureDef;
                fixtureDef.shape = &circleShape;
                fixtureDef.density = cc2d.Density;
                fixtureDef.friction = cc2d.Friction;
                fixtureDef.restitution = cc2d.Restitution;
                fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;
                fixtureDef.isSensor = cc2d.IsSensor;
                
                b2Fixture* fixture = body->CreateFixture(&fixtureDef);
                cc2d.RuntimeFixture = fixture;
            }
        }
    }

    void Scene::OnPhysics2DStop() {
        b2World* world = static_cast<b2World*>(m_PhysicsWorld);
        
        // Destroy all Box2D bodies
        auto view = m_Registry.view<Rigidbody2DComponent>();
        for (auto e : view) {
            Entity entity = { e, this };
            auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
            
            if (rb2d.RuntimeBody) {
                b2Body* body = static_cast<b2Body*>(rb2d.RuntimeBody);
                world->DestroyBody(body);
                rb2d.RuntimeBody = nullptr;
            }
            
            // Clear fixture pointers
            if (entity.HasComponent<BoxCollider2DComponent>()) {
                auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
                bc2d.RuntimeFixture = nullptr;
            }
            
            if (entity.HasComponent<CircleCollider2DComponent>()) {
                auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
                cc2d.RuntimeFixture = nullptr;
            }
        }
    }

    void Scene::OnPhysics2DUpdate(TimeStep ts) {
        // Fixed timestep physics
        const int32_t velocityIterations = 6;
        const int32_t positionIterations = 2;
        
        b2World* world = static_cast<b2World*>(m_PhysicsWorld);
        world->Step(ts, velocityIterations, positionIterations);
        
        // Retrieve transform from Box2D
        auto view = m_Registry.view<TransformComponent, Rigidbody2DComponent>();
        for (auto e : view) {
            Entity entity = { e, this };
            auto& transform = entity.GetComponent<TransformComponent>();
            auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
            
            b2Body* body = static_cast<b2Body*>(rb2d.RuntimeBody);
            const auto& position = body->GetPosition();
            transform.Position.x = position.x;
            transform.Position.y = position.y;
            transform.Rotation.z = body->GetAngle();
            
            // Update velocity in component
            const auto& velocity = body->GetLinearVelocity();
            rb2d.Velocity = { velocity.x, velocity.y };
            rb2d.AngularVelocity = body->GetAngularVelocity();
        }
    }

}

