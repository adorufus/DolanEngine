#include "Engine/Scene/Scene.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Scene/Components.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Physics/ContactListener.h"
#include "Engine/Scripting/ScriptEngine.h"
#include "Engine/Core/Logger.h"
#include <box2d/box2d.h>
#include <glm/gtc/constants.hpp>

namespace Engine {

    Scene::Scene(const std::string& name)
        : m_Name(name) {
        // Create physics world
        b2Vec2 gravity(0.0f, -9.81f);
        m_PhysicsWorld = new b2World(gravity);
        
        m_ContactListener = new ContactListener();
        static_cast<b2World*>(m_PhysicsWorld)->SetContactListener(m_ContactListener);
        
        // Create particle system
        m_ParticleSystem = CreateScope<ParticleSystem>(10000);
    }

    Scene::~Scene() {
        // Clean up physics world
        if (m_PhysicsWorld) {
            delete static_cast<b2World*>(m_PhysicsWorld);
            m_PhysicsWorld = nullptr;
        }
        
        if (m_ContactListener) {
            delete m_ContactListener;
            m_ContactListener = nullptr;
        }
    }

    Entity Scene::CreateEntity(const std::string& name) {
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<TagComponent>(name);
        entity.AddComponent<TransformComponent>();
        return entity;
    }

    void Scene::DestroyEntity(Entity entity) {
        // Call OnDestroy if entity has a script
        if (auto* script = m_Registry.try_get<ScriptComponent>((entt::entity)entity)) {
            if (script->OnDestroy) {
                auto result = script->OnDestroy(script->Instance);
                if (!result.valid()) {
                    sol::error err = result;
                    GE_CORE_ERROR("Error in OnDestroy: {}", err.what());
                }
            }
        }
        
        m_Registry.destroy(entity);
    }

    void Scene::OnStart() {
        OnPhysics2DStart();
        
        // Initialize scripts
        auto view = m_Registry.view<ScriptComponent>();
        for (auto entity : view) {
            auto& script = view.get<ScriptComponent>(entity);
            
            // Load script file
            if (!script.ScriptPath.empty() && ScriptEngine::ExecuteFile(script.ScriptPath)) {
                auto& lua = ScriptEngine::GetLuaState();
                
                // Check if script has a "Create" function and call it
                script.Instance = lua.create_table();
                script.Instance["entity"] = Entity{ entity, this };
                
                // Check for callback functions
                sol::optional<sol::protected_function> onCreate = lua["OnCreate"];
                if (onCreate) {
                    script.OnCreate = onCreate.value();
                    auto result = script.OnCreate(script.Instance);
                    if (!result.valid()) {
                        sol::error err = result;
                        GE_CORE_ERROR("Error in OnCreate: {}", err.what());
                    }
                }
                
                sol::optional<sol::protected_function> onUpdate = lua["OnUpdate"];
                if (onUpdate) {
                    script.OnUpdate = onUpdate.value();
                }
                
                sol::optional<sol::protected_function> onDestroy = lua["OnDestroy"];
                if (onDestroy) {
                    script.OnDestroy = onDestroy.value();
                }
            }
        }
        
        GE_CORE_INFO("Scene '{0}' started", m_Name);
    }

    void Scene::OnUpdate(TimeStep ts) {
        // Update animations
        {
            auto view = m_Registry.view<AnimationComponent>();
            for (auto entity : view) {
                auto& anim = view.get<AnimationComponent>(entity);
                anim.Animator.Update(ts);
                
                // Update sprite renderer with current animation frame
                auto* sprite = m_Registry.try_get<SpriteRendererComponent>((entt::entity)entity);
                if (sprite) {
                    auto texture = anim.Animator.GetCurrentTexture();
                    if (texture) {
                        sprite->SubTexture = texture;
                    }
                }
            }
        }
        
        // Update particle emitters
        {
            auto view = m_Registry.view<TransformComponent, ParticleEmitterComponent>();
            for (auto entity : view) {
                auto& transform = view.get<TransformComponent>(entity);
                auto& emitter = view.get<ParticleEmitterComponent>(entity);
                
                if (!emitter.Emit)
                    continue;
                
                emitter.Properties.Position = { transform.Position.x, transform.Position.y };
                
                if (emitter.BurstMode) {
                    m_ParticleSystem->EmitBurst(emitter.Properties, emitter.BurstCount);
                    emitter.Emit = false; // One-time burst
                } else {
                    // Continuous emission
                    emitter.EmissionTimer += (float)ts;
                    float interval = 1.0f / emitter.EmissionRate;
                    
                    while (emitter.EmissionTimer >= interval) {
                        m_ParticleSystem->Emit(emitter.Properties);
                        emitter.EmissionTimer -= interval;
                    }
                }
            }
        }
        
        // Update particle system
        m_ParticleSystem->OnUpdate(ts);
        
        // Update scripts
        {
            auto view = m_Registry.view<ScriptComponent>();
            for (auto entity : view) {
                auto& script = view.get<ScriptComponent>(entity);
                
                if (script.OnUpdate) {
                    auto result = script.OnUpdate(script.Instance, (float)ts);
                    if (!result.valid()) {
                        sol::error err = result;
                        GE_CORE_ERROR("Error in OnUpdate: {}", err.what());
                    }
                }
            }
        }
        
        // Update Box2D physics
        OnPhysics2DUpdate(ts);
        
        // Check world bounds for entities
        if (m_UseWorldBounds) {
            auto view = m_Registry.view<TransformComponent>();
            for (auto entity : view) {
                Entity e = { entity, this };
                if (!IsEntityInBounds(e)) {
                    auto& transform = e.GetComponent<TransformComponent>();
                    
                    // Clamp position within bounds
                    transform.Position.x = glm::clamp(transform.Position.x, m_WorldBoundsMin.x, m_WorldBoundsMax.x);
                    transform.Position.y = glm::clamp(transform.Position.y, m_WorldBoundsMin.y, m_WorldBoundsMax.y);
                    
                    // Reset velocity if entity has rigidbody
                    if (e.HasComponent<RigidbodyComponent>()) {
                        auto& rb = e.GetComponent<RigidbodyComponent>();
                        rb.Velocity = { 0.0f, 0.0f };
                    }
                    
                    // Reset Box2D velocity if entity has rigidbody2d
                    if (e.HasComponent<Rigidbody2DComponent>()) {
                        auto& rb2d = e.GetComponent<Rigidbody2DComponent>();
                        if (rb2d.RuntimeBody) {
                            b2Body* body = static_cast<b2Body*>(rb2d.RuntimeBody);
                            body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
                            body->SetTransform(b2Vec2(transform.Position.x, transform.Position.y), transform.Rotation.z);
                        }
                    }
                }
            }
        }
        
        // Update simple physics (legacy, for entities without Box2D)
        {
            auto view = m_Registry.view<TransformComponent, RigidbodyComponent>();
            for (auto entity : view) {
                auto& transform = view.get<TransformComponent>(entity);
                auto& rb = view.get<RigidbodyComponent>(entity);
                
                // Apply gravity
                if (rb.UseGravity) {
                    rb.Acceleration.y = -9.81f;
                }
                
                // Update velocity
                rb.Velocity += rb.Acceleration * (float)ts;
                
                // Apply drag
                rb.Velocity *= (1.0f - rb.Drag * (float)ts);
                
                // Update position
                transform.Position.x += rb.Velocity.x * (float)ts;
                transform.Position.y += rb.Velocity.y * (float)ts;
                
                // Reset acceleration
                rb.Acceleration = { 0.0f, 0.0f };
            }
        }
    }

    void Scene::OnRender() {
        // Find primary camera
        OrthographicCamera* mainCamera = nullptr;
        glm::mat4 cameraTransform;
        {
            auto view = m_Registry.view<TransformComponent, CameraComponent>();
            for (auto entity : view) {
                auto& camera = view.get<CameraComponent>(entity);
                if (camera.Primary) {
                    mainCamera = &camera.Camera;
                    cameraTransform = view.get<TransformComponent>(entity).GetTransform();
                    break;
                }
            }
        }
        
        if (mainCamera) {
            Renderer2D::BeginScene(*mainCamera);
            
            // Render sprites
            auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            size_t count = 0;
            for (auto entity : group) {
                count++;
                Entity e = { entity, this };
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                
                // Check for rotation (Box2D uses radians)
                bool hasRotation = std::abs(transform.Rotation.z) > 0.001f;
                float rotationDegrees = glm::degrees(transform.Rotation.z);
                
                // Check if entity has CircleCollider to render as circle
                bool isCircle = e.HasComponent<CircleCollider2DComponent>();
                
                if (isCircle) {
                    // Draw as circle
                    auto& circleCollider = e.GetComponent<CircleCollider2DComponent>();
                    float radius = circleCollider.Radius * std::max(transform.Scale.x, transform.Scale.y);
                    Renderer2D::DrawCircle(transform.Position, radius, sprite.Color);
                } else if (hasRotation) {
                    // Draw with rotation
                    if (sprite.SubTexture) {
                        Renderer2D::DrawRotatedQuad(transform.Position, {transform.Scale.x, transform.Scale.y}, 
                                                   rotationDegrees, sprite.SubTexture->GetTexture(), 
                                                   sprite.TilingFactor, sprite.Color);
                    } else if (sprite.Texture) {
                        Renderer2D::DrawRotatedQuad(transform.Position, {transform.Scale.x, transform.Scale.y}, 
                                                   rotationDegrees, sprite.Texture, 
                                                   sprite.TilingFactor, sprite.Color);
                    } else {
                        Renderer2D::DrawRotatedQuad(transform.Position, {transform.Scale.x, transform.Scale.y}, 
                                                   rotationDegrees, sprite.Color);
                    }
                } else {
                    // Draw without rotation
                    if (sprite.SubTexture) {
                        Renderer2D::DrawQuad(transform.Position, {transform.Scale.x, transform.Scale.y}, 
                                            sprite.SubTexture->GetTexture(), sprite.TilingFactor, sprite.Color);
                    } else if (sprite.Texture) {
                        Renderer2D::DrawQuad(transform.Position, {transform.Scale.x, transform.Scale.y}, 
                                            sprite.Texture, sprite.TilingFactor, sprite.Color);
                    } else {
                        Renderer2D::DrawQuad(transform.Position, {transform.Scale.x, transform.Scale.y}, sprite.Color);
                    }
                }
            }
            
            // Render particles
            m_ParticleSystem->OnRender();
            
            Renderer2D::EndScene();
        }
    }

    void Scene::OnStop() {
        OnPhysics2DStop();
        GE_CORE_INFO("Scene '{0}' stopped", m_Name);
    }
    
    void Scene::SetGravity(const glm::vec2& gravity) {
        if (m_PhysicsWorld) {
            b2World* world = static_cast<b2World*>(m_PhysicsWorld);
            world->SetGravity(b2Vec2(gravity.x, gravity.y));
        }
    }
    
    glm::vec2 Scene::GetGravity() const {
        if (m_PhysicsWorld) {
            b2World* world = static_cast<b2World*>(m_PhysicsWorld);
            b2Vec2 gravity = world->GetGravity();
            return { gravity.x, gravity.y };
        }
        return { 0.0f, -9.81f };
    }
    
    void Scene::SetWorldBounds(const glm::vec2& min, const glm::vec2& max) {
        m_WorldBoundsMin = min;
        m_WorldBoundsMax = max;
        m_UseWorldBounds = true;
        GE_CORE_INFO("Scene world bounds set: min=({0}, {1}), max=({2}, {3})", 
                     min.x, min.y, max.x, max.y);
    }
    
    bool Scene::IsEntityInBounds(Entity entity) const {
        if (!m_UseWorldBounds)
            return true;
            
        if (!entity.HasComponent<TransformComponent>())
            return true;
            
        auto& transform = entity.GetComponent<TransformComponent>();
        
        return transform.Position.x >= m_WorldBoundsMin.x &&
               transform.Position.x <= m_WorldBoundsMax.x &&
               transform.Position.y >= m_WorldBoundsMin.y &&
               transform.Position.y <= m_WorldBoundsMax.y;
    }

    void Scene::OnViewportResize(uint32_t width, uint32_t height) {
        m_ViewportWidth = width;
        m_ViewportHeight = height;
        
        // Resize cameras
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view) {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if (!cameraComponent.FixedAspectRatio) {
                float aspectRatio = (float)width / (float)height;
                cameraComponent.Camera.SetProjection(-aspectRatio, aspectRatio, -1.0f, 1.0f);
            }
        }
    }

    void Scene::CreatePhysicsBody(Entity entity) {
        if (!m_PhysicsWorld) {
            GE_CORE_ERROR("Cannot create physics body: Physics world not initialized");
            return;
        }
        
        if (!entity.HasComponent<Rigidbody2DComponent>()) {
            GE_CORE_WARN("Cannot create physics body: Entity missing Rigidbody2DComponent");
            return;
        }
            
        auto& transform = entity.GetComponent<TransformComponent>();
        auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
        
        // If body already exists, don't create again
        if (rb2d.RuntimeBody != nullptr) {
            GE_CORE_WARN("Physics body already exists for this entity");
            return;
        }
        
        b2World* world = static_cast<b2World*>(m_PhysicsWorld);
        
        // Convert body type
        b2BodyType b2Type = b2_staticBody;
        switch (rb2d.Type) {
            case Rigidbody2DComponent::BodyType::Static:    b2Type = b2_staticBody; break;
            case Rigidbody2DComponent::BodyType::Dynamic:   b2Type = b2_dynamicBody; break;
            case Rigidbody2DComponent::BodyType::Kinematic: b2Type = b2_kinematicBody; break;
        }
        
        b2BodyDef bodyDef;
        bodyDef.type = b2Type;
        bodyDef.position.Set(transform.Position.x, transform.Position.y);
        bodyDef.angle = transform.Rotation.z;
        bodyDef.fixedRotation = rb2d.FixedRotation;
        
        b2Body* body = world->CreateBody(&bodyDef);
        if (!body) {
            GE_CORE_ERROR("Failed to create Box2D body");
            return;
        }
        
        body->SetGravityScale(rb2d.GravityScale);
        body->SetLinearDamping(rb2d.LinearDrag);
        body->SetAngularDamping(rb2d.AngularDrag);
        
        rb2d.RuntimeBody = body;
        
        // Create collider if exists
        if (entity.HasComponent<BoxCollider2DComponent>()) {
            auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
            
            // Validate collider size
            if (bc2d.Size.x <= 0.0f || bc2d.Size.y <= 0.0f) {
                GE_CORE_WARN("BoxCollider2D has invalid size, using default 0.5x0.5");
                bc2d.Size = { 0.5f, 0.5f };
            }
            
            b2PolygonShape boxShape;
            boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, 
                              bc2d.Size.y * transform.Scale.y,
                              b2Vec2(bc2d.Offset.x, bc2d.Offset.y), 0.0f);
            
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &boxShape;
            fixtureDef.density = glm::max(bc2d.Density, 0.0f);
            fixtureDef.friction = glm::clamp(bc2d.Friction, 0.0f, 1.0f);
            fixtureDef.restitution = glm::clamp(bc2d.Restitution, 0.0f, 1.0f);
            fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
            fixtureDef.isSensor = bc2d.IsSensor;
            
            b2Fixture* fixture = body->CreateFixture(&fixtureDef);
            if (!fixture) {
                GE_CORE_ERROR("Failed to create Box2D fixture for BoxCollider2D");
            } else {
                bc2d.RuntimeFixture = fixture;
            }
        }
        
        if (entity.HasComponent<CircleCollider2DComponent>()) {
            auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();
            
            // Validate collider radius
            if (cc2d.Radius <= 0.0f) {
                GE_CORE_WARN("CircleCollider2D has invalid radius, using default 0.5");
                cc2d.Radius = 0.5f;
            }
            
            b2CircleShape circleShape;
            circleShape.m_radius = cc2d.Radius * std::max(transform.Scale.x, transform.Scale.y);
            circleShape.m_p.Set(cc2d.Offset.x, cc2d.Offset.y);
            
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &circleShape;
            fixtureDef.density = glm::max(cc2d.Density, 0.0f);
            fixtureDef.friction = glm::clamp(cc2d.Friction, 0.0f, 1.0f);
            fixtureDef.restitution = glm::clamp(cc2d.Restitution, 0.0f, 1.0f);
            fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;
            fixtureDef.isSensor = cc2d.IsSensor;
            
            b2Fixture* fixture = body->CreateFixture(&fixtureDef);
            if (!fixture) {
                GE_CORE_ERROR("Failed to create Box2D fixture for CircleCollider2D");
            } else {
                cc2d.RuntimeFixture = fixture;
            }
        }
    }

    template<typename T>
    void Scene::OnComponentAdded(Entity entity, T& component) {
        // Default implementation - do nothing
    }
    
    // Explicit template instantiations for all component types
    template void Scene::OnComponentAdded<TagComponent>(Entity, TagComponent&);
    template void Scene::OnComponentAdded<TransformComponent>(Entity, TransformComponent&);
    template void Scene::OnComponentAdded<CameraComponent>(Entity, CameraComponent&);
    template void Scene::OnComponentAdded<SpriteRendererComponent>(Entity, SpriteRendererComponent&);
    template void Scene::OnComponentAdded<ScriptComponent>(Entity, ScriptComponent&);
    template void Scene::OnComponentAdded<AnimationComponent>(Entity, AnimationComponent&);
    template void Scene::OnComponentAdded<ParticleEmitterComponent>(Entity, ParticleEmitterComponent&);
    template void Scene::OnComponentAdded<AudioSourceComponent>(Entity, AudioSourceComponent&);
    template void Scene::OnComponentAdded<AudioListenerComponent>(Entity, AudioListenerComponent&);
    
    // Specialization for Rigidbody2DComponent - create physics body
    template<>
    void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component) {
        CreatePhysicsBody(entity);
    }
    
    // Specialization for BoxCollider2DComponent - update physics body if exists
    template<>
    void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component) {
        if (entity.HasComponent<Rigidbody2DComponent>()) {
            // Body exists, need to add fixture
            auto& rb = entity.GetComponent<Rigidbody2DComponent>();
            auto& transform = entity.GetComponent<TransformComponent>();
            
            if (rb.RuntimeBody) {
                b2Body* body = static_cast<b2Body*>(rb.RuntimeBody);
                
                b2PolygonShape boxShape;
                boxShape.SetAsBox(component.Size.x * transform.Scale.x, 
                                  component.Size.y * transform.Scale.y,
                                  b2Vec2(component.Offset.x, component.Offset.y), 0.0f);
                
                b2FixtureDef fixtureDef;
                fixtureDef.shape = &boxShape;
                fixtureDef.density = component.Density;
                fixtureDef.friction = component.Friction;
                fixtureDef.restitution = component.Restitution;
                fixtureDef.restitutionThreshold = component.RestitutionThreshold;
                fixtureDef.isSensor = component.IsSensor;
                
                b2Fixture* fixture = body->CreateFixture(&fixtureDef);
                component.RuntimeFixture = fixture;
            }
        }
    }
    
    // Specialization for CircleCollider2DComponent  
    template<>
    void Scene::OnComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component) {
        if (entity.HasComponent<Rigidbody2DComponent>()) {
            auto& rb = entity.GetComponent<Rigidbody2DComponent>();
            auto& transform = entity.GetComponent<TransformComponent>();
            
            if (rb.RuntimeBody) {
                b2Body* body = static_cast<b2Body*>(rb.RuntimeBody);
                
                b2CircleShape circleShape;
                circleShape.m_radius = component.Radius * std::max(transform.Scale.x, transform.Scale.y);
                circleShape.m_p.Set(component.Offset.x, component.Offset.y);
                
                b2FixtureDef fixtureDef;
                fixtureDef.shape = &circleShape;
                fixtureDef.density = component.Density;
                fixtureDef.friction = component.Friction;
                fixtureDef.restitution = component.Restitution;
                fixtureDef.restitutionThreshold = component.RestitutionThreshold;
                fixtureDef.isSensor = component.IsSensor;
                
                b2Fixture* fixture = body->CreateFixture(&fixtureDef);
                component.RuntimeFixture = fixture;
            }
        }
    }

}

