# Quick Start Guide - Phase 3 Features

## Entity-Component System (ECS)

### Creating a Scene
```cpp
#include <Engine.h>

// Create scene
auto scene = Engine::CreateRef<Engine::Scene>("MyScene");
scene->OnStart();
```

### Creating Entities
```cpp
// Create player entity
auto player = scene->CreateEntity("Player");

// Add transform
auto& transform = player.GetComponent<Engine::TransformComponent>();
transform.Position = { 0.0f, 0.0f, 0.0f };
transform.Scale = { 1.0f, 1.0f, 1.0f };

// Add sprite
auto& sprite = player.AddComponent<Engine::SpriteRendererComponent>();
sprite.Color = { 1.0f, 0.0f, 0.0f, 1.0f }; // Red
```

### Working with Components
```cpp
// Check if entity has component
if (player.HasComponent<Engine::SpriteRendererComponent>()) {
    // Get component
    auto& sprite = player.GetComponent<Engine::SpriteRendererComponent>();
    sprite.Color = { 0.0f, 1.0f, 0.0f, 1.0f }; // Change to green
}

// Remove component
player.RemoveComponent<Engine::SpriteRendererComponent>();
```

### Available Components
```cpp
// Tag (name)
auto& tag = entity.GetComponent<Engine::TagComponent>();
tag.Tag = "Enemy";

// Transform (position, rotation, scale)
auto& transform = entity.GetComponent<Engine::TransformComponent>();
transform.Position = { x, y, z };
transform.Rotation = { rx, ry, rz };
transform.Scale = { sx, sy, sz };

// Sprite Renderer (visual)
auto& sprite = entity.AddComponent<Engine::SpriteRendererComponent>();
sprite.Color = { r, g, b, a };
sprite.Texture = myTexture;
sprite.TilingFactor = 1.0f;

// Camera
auto& camera = entity.AddComponent<Engine::CameraComponent>();
camera.Camera.SetProjection(-10.0f, 10.0f, -5.0f, 5.0f);
camera.Primary = true;

// Rigidbody (physics)
auto& rb = entity.AddComponent<Engine::RigidbodyComponent>();
rb.Velocity = { 0.0f, 0.0f };
rb.UseGravity = true;
rb.Mass = 1.0f;
```

## Scene Management

### Scene Lifecycle
```cpp
// In your Layer class
class GameLayer : public Engine::Layer {
public:
    GameLayer() : Layer("Game") {
        m_Scene = Engine::CreateRef<Engine::Scene>("Level1");
        m_Scene->OnStart();
    }
    
    void OnUpdate(Engine::TimeStep ts) override {
        m_Scene->OnUpdate(ts);
    }
    
    void OnRender() override {
        m_Scene->OnRender();
    }
    
    ~GameLayer() {
        m_Scene->OnStop();
    }
    
private:
    Engine::Ref<Engine::Scene> m_Scene;
};
```

## Asset Management

### Loading Assets
```cpp
// Load texture with caching
auto texture = Engine::AssetManager::LoadTexture("player", "assets/player.png");

// Load shader with caching
auto shader = Engine::AssetManager::LoadShader("basic", "assets/basic.glsl");

// Retrieve cached asset
auto tex = Engine::AssetManager::GetTexture("player");

// Check if asset exists
if (Engine::AssetManager::HasTexture("player")) {
    // Use texture
}

// Get statistics
auto stats = Engine::AssetManager::GetStats();
GE_INFO("Loaded {} textures, {} shaders", 
        stats.TexturesLoaded, stats.ShadersLoaded);
```

## Sprite Sheets (SubTexture2D)

### Using Sprite Sheets
```cpp
// Load the sprite sheet
auto spriteSheet = Engine::Texture2D::Create("assets/spritesheet.png");

// Extract a single sprite (grid-based)
auto sprite = Engine::SubTexture2D::CreateFromCoords(
    spriteSheet,
    {2, 3},        // Grid position (column 2, row 3)
    {32, 32},      // Cell size (32x32 pixels)
    {1, 1}         // Sprite size (1x1 cells)
);

// Extract a larger sprite (2x2 cells)
auto bigSprite = Engine::SubTexture2D::CreateFromCoords(
    spriteSheet,
    {0, 0},        // Starting position
    {32, 32},      // Cell size
    {2, 2}         // 2x2 cells (64x64 pixels)
);

// Use in entity
auto& sprite = entity.GetComponent<Engine::SpriteRendererComponent>();
sprite.SubTexture = sprite;
```

## Scene Serialization

### Saving Scenes
```cpp
Engine::SceneSerializer serializer(scene);
serializer.Serialize("assets/scenes/level1.scene");
```

### Loading Scenes
```cpp
auto scene = Engine::CreateRef<Engine::Scene>("LoadedScene");
Engine::SceneSerializer serializer(scene);

if (serializer.Deserialize("assets/scenes/level1.scene")) {
    GE_INFO("Scene loaded successfully");
} else {
    GE_ERROR("Failed to load scene");
}
```

### Scene File Format (YAML)
```yaml
Scene: Level 1
Entities:
  - Entity: 0
    TagComponent:
      Tag: Player
    TransformComponent:
      Position: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    SpriteRendererComponent:
      Color: [1, 0, 0, 1]
      TilingFactor: 1
```

## Layer System

### Creating a Layer
```cpp
class MyGameLayer : public Engine::Layer {
public:
    MyGameLayer() : Layer("MyGame") {
        // Initialize
        m_Scene = Engine::CreateRef<Engine::Scene>("Game");
        m_Scene->OnStart();
    }
    
    void OnAttach() override {
        // Called when layer is added
        GE_INFO("Layer attached");
    }
    
    void OnDetach() override {
        // Called when layer is removed
        m_Scene->OnStop();
    }
    
    void OnUpdate(Engine::TimeStep ts) override {
        // Update game logic
        m_Scene->OnUpdate(ts);
    }
    
    void OnRender() override {
        // Render
        m_Scene->OnRender();
    }
    
    void OnEvent(Engine::Event& event) override {
        // Handle events
        if (event.GetEventType() == Engine::EventType::KeyPressed) {
            // Handle key press
        }
    }
    
private:
    Engine::Ref<Engine::Scene> m_Scene;
};
```

### Using Layers in Application
```cpp
class MyApp : public Engine::Application {
public:
    MyApp() : Application("My Game") {
        PushLayer(new MyGameLayer());
    }
};

Engine::Application* Engine::CreateApplication() {
    return new MyApp();
}
```

## Complete Example

```cpp
#include <Engine.h>

class GameLayer : public Engine::Layer {
public:
    GameLayer() : Layer("Game") {
        // Create scene
        m_Scene = Engine::CreateRef<Engine::Scene>("GameScene");
        
        // Create player
        auto player = m_Scene->CreateEntity("Player");
        auto& playerTransform = player.GetComponent<Engine::TransformComponent>();
        playerTransform.Position = { 0.0f, 0.0f, 0.0f };
        playerTransform.Scale = { 1.0f, 1.0f, 1.0f };
        
        auto& playerSprite = player.AddComponent<Engine::SpriteRendererComponent>();
        playerSprite.Color = { 1.0f, 0.0f, 0.0f, 1.0f };
        
        // Create camera
        auto camera = m_Scene->CreateEntity("Camera");
        auto& cam = camera.AddComponent<Engine::CameraComponent>();
        cam.Camera.SetProjection(-10.0f, 10.0f, -5.0f, 5.0f);
        cam.Primary = true;
        
        // Start scene
        m_Scene->OnStart();
        
        // Load assets
        m_Texture = Engine::AssetManager::LoadTexture("player", "assets/player.png");
    }
    
    void OnUpdate(Engine::TimeStep ts) override {
        // Update scene
        m_Scene->OnUpdate(ts);
        
        // Custom logic
        m_Time += ts;
    }
    
    void OnRender() override {
        m_Scene->OnRender();
    }
    
    void OnEvent(Engine::Event& event) override {
        // Handle input
    }
    
    ~GameLayer() {
        m_Scene->OnStop();
    }
    
private:
    Engine::Ref<Engine::Scene> m_Scene;
    Engine::Ref<Engine::Texture2D> m_Texture;
    float m_Time = 0.0f;
};

class MyGame : public Engine::Application {
public:
    MyGame() : Application("My 2D Game") {
        PushLayer(new GameLayer());
    }
};

Engine::Application* Engine::CreateApplication() {
    return new MyGame();
}
```

## Building Your Game

```bash
# Navigate to project
cd GameEngine

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build
cmake --build .

# Run
./bin/Sandbox
```

## Performance Tips

1. **Batch Similar Entities**: Group entities with same textures together
2. **Use Asset Manager**: Always cache frequently used assets
3. **Minimize Component Queries**: Cache entity references when possible
4. **Sprite Sheets**: Use texture atlases instead of individual textures
5. **Scene Serialization**: Save/load scenes for faster level loading

## Common Patterns

### Player Controller
```cpp
// In OnUpdate
if (Engine::Input::IsKeyPressed(Engine::KeyCode::W))
    transform.Position.y += speed * ts;
if (Engine::Input::IsKeyPressed(Engine::KeyCode::S))
    transform.Position.y -= speed * ts;
if (Engine::Input::IsKeyPressed(Engine::KeyCode::A))
    transform.Position.x -= speed * ts;
if (Engine::Input::IsKeyPressed(Engine::KeyCode::D))
    transform.Position.x += speed * ts;
```

### Spawning Entities
```cpp
for (int i = 0; i < 10; i++) {
    auto enemy = m_Scene->CreateEntity("Enemy");
    auto& t = enemy.GetComponent<Engine::TransformComponent>();
    t.Position = { randomX(), randomY(), 0.0f };
    
    auto& s = enemy.AddComponent<Engine::SpriteRendererComponent>();
    s.Texture = enemyTexture;
}
```

### Destroying Entities
```cpp
m_Scene->DestroyEntity(enemy);
```

---

Ready to build your 2D game! 🎮

