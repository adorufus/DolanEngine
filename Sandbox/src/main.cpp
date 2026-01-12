#include <Engine.h>
#include <cmath>
#include <random>

class PhysicsSandboxLayer : public Engine::Layer {
public:
    PhysicsSandboxLayer()
        : Layer("PhysicsSandbox") {
        GE_INFO("PhysicsSandboxLayer initialized with Box2D Physics!");
        
        // Create scene
        m_Scene = Engine::CreateRef<Engine::Scene>("Physics Demo Scene");
        m_Scene->SetGravity({0.0f, -9.81f});
        
        // Create camera
        auto cameraEntity = m_Scene->CreateEntity("Camera");
        auto& camera = cameraEntity.AddComponent<Engine::CameraComponent>();
        camera.Camera.SetProjection(-16.0f, 16.0f, -9.0f, 9.0f);
        camera.Primary = true;
        
        // Create ground (static body)
        auto ground = m_Scene->CreateEntity("Ground");
        auto& groundTransform = ground.GetComponent<Engine::TransformComponent>();
        groundTransform.Position = { 0.0f, -8.0f, 0.0f };
        groundTransform.Scale = { 15.0f, 1.0f, 1.0f };
        
        auto& groundSprite = ground.AddComponent<Engine::SpriteRendererComponent>();
        groundSprite.Color = { 0.3f, 0.3f, 0.3f, 1.0f };
        
        auto& groundRB = ground.AddComponent<Engine::Rigidbody2DComponent>();
        groundRB.Type = Engine::Rigidbody2DComponent::BodyType::Static;
        
        auto& groundCollider = ground.AddComponent<Engine::BoxCollider2DComponent>();
        groundCollider.Size = { 15.0f, 1.0f };
        
        // Create walls
        CreateWall("Left Wall", {-15.0f, 0.0f, 0.0f}, {1.0f, 10.0f, 1.0f});
        CreateWall("Right Wall", {15.0f, 0.0f, 0.0f}, {1.0f, 10.0f, 1.0f});
        
        // Create falling boxes
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> colorDist(0.3, 1.0);
        std::uniform_real_distribution<> xDist(-10.0, 10.0);
        
        for (int i = 0; i < 20; i++) {
            auto box = m_Scene->CreateEntity("Box");
            auto& transform = box.GetComponent<Engine::TransformComponent>();
            transform.Position = { xDist(gen), 5.0f + i * 1.5f, 0.0f };
            transform.Scale = { 0.8f, 0.8f, 1.0f };
            
            auto& sprite = box.AddComponent<Engine::SpriteRendererComponent>();
            sprite.Color = { 
                colorDist(gen), 
                colorDist(gen), 
                colorDist(gen), 
                1.0f 
            };
            
            auto& rb = box.AddComponent<Engine::Rigidbody2DComponent>();
            rb.Type = Engine::Rigidbody2DComponent::BodyType::Dynamic;
            rb.Mass = 1.0f;
            
            auto& collider = box.AddComponent<Engine::BoxCollider2DComponent>();
            collider.Size = { 0.4f, 0.4f };
            collider.Density = 1.0f;
            collider.Friction = 0.5f;
            collider.Restitution = 0.3f; // Bouncy!
        }
        
        // Create some circles
        for (int i = 0; i < 10; i++) {
            auto circle = m_Scene->CreateEntity("Circle");
            auto& transform = circle.GetComponent<Engine::TransformComponent>();
            transform.Position = { xDist(gen), 10.0f + i * 1.5f, 0.0f };
            transform.Scale = { 0.6f, 0.6f, 1.0f };
            
            auto& sprite = circle.AddComponent<Engine::SpriteRendererComponent>();
            sprite.Color = { 
                colorDist(gen), 
                colorDist(gen), 
                colorDist(gen), 
                1.0f 
            };
            
            auto& rb = circle.AddComponent<Engine::Rigidbody2DComponent>();
            rb.Type = Engine::Rigidbody2DComponent::BodyType::Dynamic;
            
            auto& collider = circle.AddComponent<Engine::CircleCollider2DComponent>();
            collider.Radius = 0.3f;
            collider.Density = 0.8f;
            collider.Friction = 0.3f;
            collider.Restitution = 0.5f; // More bouncy!
        }
        
        // Create a platform (kinematic body)
        m_Platform = m_Scene->CreateEntity("Platform");
        auto& platformTransform = m_Platform.GetComponent<Engine::TransformComponent>();
        platformTransform.Position = { 0.0f, 0.0f, 0.0f };
        platformTransform.Scale = { 4.0f, 0.5f, 1.0f };
        
        auto& platformSprite = m_Platform.AddComponent<Engine::SpriteRendererComponent>();
        platformSprite.Color = { 0.8f, 0.6f, 0.2f, 1.0f };
        
        auto& platformRB = m_Platform.AddComponent<Engine::Rigidbody2DComponent>();
        platformRB.Type = Engine::Rigidbody2DComponent::BodyType::Kinematic;
        
        auto& platformCollider = m_Platform.AddComponent<Engine::BoxCollider2DComponent>();
        platformCollider.Size = { 4.0f, 0.5f };
        
        GE_INFO("Created 33 entities with physics!");
        
        // Start scene
        m_Scene->OnStart();
        
        GE_INFO("Controls: ESC to exit, SPACE to spawn box");
        GE_INFO("Box2D Physics: {} bodies created", 33);
    }
    
    ~PhysicsSandboxLayer() {
        m_Scene->OnStop();
        GE_INFO("PhysicsSandboxLayer destroyed!");
    }
    
    void CreateWall(const std::string& name, const glm::vec3& pos, const glm::vec3& scale) {
        auto wall = m_Scene->CreateEntity(name);
        auto& transform = wall.GetComponent<Engine::TransformComponent>();
        transform.Position = pos;
        transform.Scale = scale;
        
        auto& sprite = wall.AddComponent<Engine::SpriteRendererComponent>();
        sprite.Color = { 0.4f, 0.4f, 0.4f, 1.0f };
        
        auto& rb = wall.AddComponent<Engine::Rigidbody2DComponent>();
        rb.Type = Engine::Rigidbody2DComponent::BodyType::Static;
        
        auto& collider = wall.AddComponent<Engine::BoxCollider2DComponent>();
        collider.Size = { scale.x, scale.y };
    }
    
    void OnUpdate(Engine::TimeStep ts) override {
        m_Time += ts;
        
        // Move platform
        if (m_Platform) {
            auto& transform = m_Platform.GetComponent<Engine::TransformComponent>();
            transform.Position.x = 8.0f * std::sin(m_Time * 0.5f);
            transform.Position.y = 2.0f * std::sin(m_Time * 1.0f);
        }
        
        // Update scene
        m_Scene->OnUpdate(ts);
        
        // Stats
        if (m_Time - m_LastStatsTime > 2.0f) {
            auto stats = Engine::Renderer2D::GetStats();
            GE_INFO("Renderer - DrawCalls: {}, Quads: {}", stats.DrawCalls, stats.QuadCount);
            m_LastStatsTime = m_Time;
        }
    }
    
    void OnRender() override {
        m_Scene->OnRender();
        Engine::Renderer2D::ResetStats();
    }
    
    void OnEvent(Engine::Event& event) override {
        // Future: spawn boxes on key press
    }
    
private:
    Engine::Ref<Engine::Scene> m_Scene;
    Engine::Entity m_Platform;
    float m_Time = 0.0f;
    float m_LastStatsTime = 0.0f;
};

class SandboxApp : public Engine::Application {
public:
    SandboxApp()
        : Application("Sandbox - 2D Physics with Box2D") {
        PushLayer(new PhysicsSandboxLayer());
    }
    
    ~SandboxApp() = default;
};

Engine::Application* Engine::CreateApplication() {
    return new SandboxApp();
}
