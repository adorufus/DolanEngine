#include "Engine/Scene/Scene.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Scene/Components.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Physics/ContactListener.h"
#include "Engine/Core/Logger.h"
#include <box2d/box2d.h>

namespace Engine {

    Scene::Scene(const std::string& name)
        : m_Name(name) {
        // Create physics world
        b2Vec2 gravity(0.0f, -9.81f);
        m_PhysicsWorld = new b2World(gravity);
        
        m_ContactListener = new ContactListener();
        static_cast<b2World*>(m_PhysicsWorld)->SetContactListener(m_ContactListener);
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
        m_Registry.destroy(entity);
    }

    void Scene::OnStart() {
        OnPhysics2DStart();
        GE_CORE_INFO("Scene '{0}' started", m_Name);
    }

    void Scene::OnUpdate(TimeStep ts) {
        // Update scripts (future)
        
        // Update Box2D physics
        OnPhysics2DUpdate(ts);
        
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
            for (auto entity : group) {
                Entity e = { entity, this };
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                
                // Check if entity has CircleCollider to render as circle
                bool isCircle = e.HasComponent<CircleCollider2DComponent>();
                
                if (isCircle) {
                    // Draw as circle
                    auto& circleCollider = e.GetComponent<CircleCollider2DComponent>();
                    float radius = circleCollider.Radius * std::max(transform.Scale.x, transform.Scale.y);
                    Renderer2D::DrawCircle(transform.Position, radius, sprite.Color);
                } else if (sprite.SubTexture) {
                    // Draw with subtexture (sprite sheet)
                    Renderer2D::DrawQuad(transform.Position, {transform.Scale.x, transform.Scale.y}, 
                                        sprite.SubTexture->GetTexture(), sprite.TilingFactor, sprite.Color);
                } else if (sprite.Texture) {
                    // Draw with texture
                    Renderer2D::DrawQuad(transform.Position, {transform.Scale.x, transform.Scale.y}, 
                                        sprite.Texture, sprite.TilingFactor, sprite.Color);
                } else {
                    // Draw colored quad
                    Renderer2D::DrawQuad(transform.Position, {transform.Scale.x, transform.Scale.y}, sprite.Color);
                }
            }
            
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

    template<typename T>
    void Scene::OnComponentAdded(Entity entity, T& component) {
        // Default implementation
    }

}

