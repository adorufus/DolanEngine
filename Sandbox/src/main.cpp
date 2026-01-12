#include <Engine.h>

class GameLayer : public Engine::Layer {
public:
    GameLayer() : Layer("GameLayer") {}

    void OnAttach() override {
        // Create scene
        m_Scene = Engine::CreateRef<Engine::Scene>("Scripting Demo");
        
        // Create camera
        auto camera = m_Scene->CreateEntity("Camera");
        auto& cameraComp = camera.AddComponent<Engine::CameraComponent>();
        cameraComp.Camera.SetProjection(-16.0f, 16.0f, -9.0f, 9.0f);
        cameraComp.Primary = true;
        
        // Create player entity with Lua script
        m_Player = m_Scene->CreateEntity("Player");
        auto& playerTransform = m_Player.GetComponent<Engine::TransformComponent>();
        playerTransform.Position = { 0.0f, 0.0f, 0.0f };
        playerTransform.Scale = { 1.0f, 1.0f, 1.0f };
        
        auto& playerSprite = m_Player.AddComponent<Engine::SpriteRendererComponent>();
        playerSprite.Color = { 0.3f, 0.7f, 1.0f, 1.0f };
        
        auto& playerRb = m_Player.AddComponent<Engine::Rigidbody2DComponent>();
        playerRb.Type = Engine::Rigidbody2DComponent::BodyType::Dynamic;
        playerRb.FixedRotation = true;
        
        auto& playerCollider = m_Player.AddComponent<Engine::BoxCollider2DComponent>();
        playerCollider.Size = { 0.4f, 0.4f };  // Half-extents
        
        // Attach Lua script to player
        auto& playerScript = m_Player.AddComponent<Engine::ScriptComponent>();
        playerScript.ScriptPath = "assets/scripts/PlayerController.lua";
        
        // Create enemy with AI script
        auto enemy = m_Scene->CreateEntity("Enemy");
        auto& enemyTransform = enemy.GetComponent<Engine::TransformComponent>();
        enemyTransform.Position = { 5.0f, 0.0f, 0.0f };
        enemyTransform.Scale = { 1.0f, 1.0f, 1.0f };
        
        auto& enemySprite = enemy.AddComponent<Engine::SpriteRendererComponent>();
        enemySprite.Color = { 1.0f, 0.3f, 0.3f, 1.0f };
        
        auto& enemyRb = enemy.AddComponent<Engine::Rigidbody2DComponent>();
        enemyRb.Type = Engine::Rigidbody2DComponent::BodyType::Dynamic;
        enemyRb.FixedRotation = true;
        
        auto& enemyCollider = enemy.AddComponent<Engine::BoxCollider2DComponent>();
        enemyCollider.Size = { 0.4f, 0.4f };  // Half-extents
        
        // Attach AI script to enemy
        auto& enemyScript = enemy.AddComponent<Engine::ScriptComponent>();
        enemyScript.ScriptPath = "assets/scripts/EnemyAI.lua";
        
        // Create rotating platform
        auto platform = m_Scene->CreateEntity("Rotating Platform");
        auto& platformTransform = platform.GetComponent<Engine::TransformComponent>();
        platformTransform.Position = { 0.0f, -3.0f, 0.0f };
        platformTransform.Scale = { 4.0f, 0.5f, 1.0f };
        
        auto& platformSprite = platform.AddComponent<Engine::SpriteRendererComponent>();
        platformSprite.Color = { 0.8f, 0.8f, 0.2f, 1.0f };
        
        auto& platformRb = platform.AddComponent<Engine::Rigidbody2DComponent>();
        platformRb.Type = Engine::Rigidbody2DComponent::BodyType::Kinematic;
        platformRb.GravityScale = 0.0f;  // No gravity on kinematic bodies
        
        auto& platformCollider = platform.AddComponent<Engine::BoxCollider2DComponent>();
        platformCollider.Size = { 0.5f, 0.125f };  // Half-extents (will be multiplied by scale 4x0.5 = 2x0.25 actual size)
        
        // Attach rotator script
        auto& platformScript = platform.AddComponent<Engine::ScriptComponent>();
        platformScript.ScriptPath = "assets/scripts/Rotator.lua";
        
        // Create ground
        auto ground = m_Scene->CreateEntity("Ground");
        auto& groundTransform = ground.GetComponent<Engine::TransformComponent>();
        groundTransform.Position = { 0.0f, -5.0f, 0.0f };
        groundTransform.Scale = { 20.0f, 1.0f, 1.0f };
        
        auto& groundSprite = ground.AddComponent<Engine::SpriteRendererComponent>();
        groundSprite.Color = { 0.5f, 0.5f, 0.5f, 1.0f };
        
        auto& groundRb = ground.AddComponent<Engine::Rigidbody2DComponent>();
        groundRb.Type = Engine::Rigidbody2DComponent::BodyType::Static;
        
        auto& groundCollider = ground.AddComponent<Engine::BoxCollider2DComponent>();
        groundCollider.Size = { 0.5f, 0.5f };  // Half-extents (will be multiplied by scale: 0.5*20=10 wide, 0.5*1=0.5 tall)
        
        // Start scene
        m_Scene->OnStart();
        
        // Initialize script reloader
        Engine::ScriptReloader::Init();
        Engine::ScriptReloader::WatchScript("assets/scripts/PlayerController.lua");
        Engine::ScriptReloader::WatchScript("assets/scripts/EnemyAI.lua");
        Engine::ScriptReloader::WatchScript("assets/scripts/Rotator.lua");
        
        // Setup reload callback
        Engine::ScriptReloader::SetReloadCallback([this](const std::string& path) {
            GE_CORE_INFO("Script reloaded, restarting scene...");
            m_Scene->OnStop();
            m_Scene->OnStart();
        });
        
        GE_CORE_INFO("=== Lua Scripting Demo ===");
        GE_CORE_INFO("Controls:");
        GE_CORE_INFO("  WASD - Move player");
        GE_CORE_INFO("  SPACE - Jump");
        GE_CORE_INFO("  E - Change player color");
        GE_CORE_INFO("  R - Reset player position");
        GE_CORE_INFO("  F5 - Reload scripts (hot-reload)");
        GE_CORE_INFO("========================");
    }

    void OnDetach() override {
        m_Scene->OnStop();
        Engine::ScriptReloader::Shutdown();
    }

    void OnUpdate(Engine::TimeStep ts) override {
        // Check for manual script reload
        if (Engine::Input::IsKeyPressed(Engine::KeyCode::F5)) {
            GE_CORE_INFO("Manual script reload triggered!");
            m_Scene->OnStop();
            m_Scene->OnStart();
        }
        
        // Check for hot-reload changes
        Engine::ScriptReloader::CheckForChanges();
        
        // Update scene
        m_Scene->OnUpdate(ts);
        
        // Render scene
        Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.15f, 1.0f });
        Engine::RenderCommand::Clear();
        
        m_Scene->OnRender();
    }

    void OnEvent(Engine::Event& event) override {
        // Handle events if needed
    }

private:
    Engine::Ref<Engine::Scene> m_Scene;
    Engine::Entity m_Player;
};

class SandboxApp : public Engine::Application {
public:
    SandboxApp() : Application("Lua Scripting Demo") {
        PushLayer(new GameLayer());
    }

    ~SandboxApp() = default;
};

Engine::Application* Engine::CreateApplication() {
    return new SandboxApp();
}
