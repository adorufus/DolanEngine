# 📚 API Reference

**Version:** 1.0.0  
**Engine:** 2D Game Engine with C++ and Lua

---

## Table of Contents

1. [Lua API](#lua-api)
2. [Component API](#component-api)
3. [Input System](#input-system)
4. [Vector Types](#vector-types)
5. [KeyCodes](#keycodes)
6. [C++ API](#c-api)

---

## 🎨 **Lua API**

### **Script Lifecycle Functions**

#### `OnCreate(self)`
Called once when the entity is created or when the script is hot-reloaded.

```lua
function OnCreate(self)
    print("Entity initialized")
    -- Initialize variables
    -- Cache component references
end
```

**Parameters:**
- `self` - Script instance reference

**Returns:** None

---

#### `OnUpdate(self, dt)`
Called every frame during game simulation.

```lua
function OnUpdate(self, dt)
    -- Update game logic
    -- Handle input
    -- Move entities
end
```

**Parameters:**
- `self` - Script instance reference
- `dt` - Delta time (time since last frame in seconds)

**Returns:** None

**Note:** Only called when game is playing (not in edit mode)

---

#### `OnDestroy(self)`
Called when the entity is destroyed or scene is unloaded.

```lua
function OnDestroy(self)
    print("Entity destroyed")
    -- Cleanup resources
end
```

**Parameters:**
- `self` - Script instance reference

**Returns:** None

---

#### `OnCollisionEnter(self, other)` *(Optional)*
Called when collision starts with another entity.

```lua
function OnCollisionEnter(self, other)
    print("Collision started")
    -- Handle collision
end
```

**Parameters:**
- `self` - Script instance reference
- `other` - Handle to the other entity (currently limited API)

**Returns:** None

**Note:** Requires Rigidbody2D and Collider components

---

#### `OnCollisionExit(self, other)` *(Optional)*
Called when collision ends with another entity.

```lua
function OnCollisionExit(self, other)
    print("Collision ended")
end
```

**Parameters:**
- `self` - Script instance reference
- `other` - Handle to the other entity

**Returns:** None

---

## 📦 **Component API**

### **Getting Components**

#### `self.entity:GetComponent_Transform()`
Returns the Transform component.

```lua
local transform = self.entity:GetComponent_Transform()
```

**Returns:** TransformComponent

---

#### `self.entity:GetComponent_SpriteRenderer()`
Returns the Sprite Renderer component.

```lua
local sprite = self.entity:GetComponent_SpriteRenderer()
```

**Returns:** SpriteRendererComponent or `nil` if not present

---

#### `self.entity:GetComponent_Rigidbody2D()`
Returns the Rigidbody2D component.

```lua
local rb = self.entity:GetComponent_Rigidbody2D()
```

**Returns:** Rigidbody2DComponent or `nil` if not present

---

#### `self.entity:GetComponent_Script()`
Returns the Script component.

```lua
local script = self.entity:GetComponent_Script()
```

**Returns:** ScriptComponent

---

#### `self.entity:GetComponent_ParticleEmitter()`
Returns the Particle Emitter component.

```lua
local emitter = self.entity:GetComponent_ParticleEmitter()
```

**Returns:** ParticleEmitterComponent or `nil` if not present

---

#### `self.entity:GetComponent_AudioSource()`
Returns the Audio Source component.

```lua
local audio = self.entity:GetComponent_AudioSource()
```

**Returns:** AudioSourceComponent or `nil` if not present

---

### **Transform Component**

Properties:

#### `Position` : `Vec3`
World position of the entity.

```lua
local transform = self.entity:GetComponent_Transform()

-- Read
local pos = transform.Position  -- Vec3
print(pos.x, pos.y, pos.z)

-- Write
transform.Position = Vec3.new(1.0, 2.0, 0.0)
```

---

#### `Rotation` : `Vec3`
Rotation in degrees (Euler angles).

```lua
-- Read
local rot = transform.Rotation  -- Vec3
print(rot.x, rot.y, rot.z)

-- Write (rotate 45 degrees around Z axis)
transform.Rotation = Vec3.new(0, 0, 45)
```

---

#### `Scale` : `Vec3`
Scale of the entity.

```lua
-- Read
local scale = transform.Scale  -- Vec3

-- Write
transform.Scale = Vec3.new(2.0, 2.0, 1.0)  -- 2x size
```

---

### **Sprite Renderer Component**

Properties:

#### `Color` : `Vec4`
RGBA color (0.0 to 1.0 for each channel).

```lua
local sprite = self.entity:GetComponent_SpriteRenderer()

-- Set to red
sprite.Color = Vec4.new(1.0, 0.0, 0.0, 1.0)

-- Set to semi-transparent blue
sprite.Color = Vec4.new(0.0, 0.0, 1.0, 0.5)
```

---

#### `TilingFactor` : `float`
Texture tiling multiplier.

```lua
sprite.TilingFactor = 2.0  -- Repeat texture 2x
```

---

### **Rigidbody2D Component**

Methods:

#### `GetVelocity()` → `Vec2`
Returns current linear velocity.

```lua
local rb = self.entity:GetComponent_Rigidbody2D()
local vel = rb:GetVelocity()
print(vel.x, vel.y)
```

---

#### `SetVelocity(velocity: Vec2)`
Sets linear velocity directly.

```lua
rb:SetVelocity(Vec2.new(5.0, 0.0))  -- Move right at 5 units/sec
```

**Parameters:**
- `velocity` : `Vec2` - New velocity

---

#### `GetAngularVelocity()` → `float`
Returns current angular velocity (radians/sec).

```lua
local angVel = rb:GetAngularVelocity()
```

---

#### `SetAngularVelocity(velocity: float)`
Sets angular velocity.

```lua
rb:SetAngularVelocity(1.5)  -- Rotate at 1.5 rad/sec
```

**Parameters:**
- `velocity` : `float` - Angular velocity in radians/sec

---

#### `ApplyForce(force: Vec2, wake: bool)`
Applies a continuous force.

```lua
rb:ApplyForce(Vec2.new(10.0, 0.0), true)
```

**Parameters:**
- `force` : `Vec2` - Force vector
- `wake` : `bool` - Wake up sleeping body

**Note:** Force is continuous, use in OnUpdate for constant push

---

#### `ApplyLinearImpulseToCenter(impulse: Vec2, wake: bool)`
Applies an instant impulse (like a jump or explosion).

```lua
rb:ApplyLinearImpulseToCenter(Vec2.new(0, 5.0), true)  -- Jump
```

**Parameters:**
- `impulse` : `Vec2` - Impulse vector
- `wake` : `bool` - Wake up sleeping body

**Note:** Impulse is instant, use once (e.g., jump, shoot)

---

#### `ApplyTorque(torque: float, wake: bool)`
Applies rotational torque.

```lua
rb:ApplyTorque(2.0, true)  -- Spin clockwise
```

**Parameters:**
- `torque` : `float` - Torque amount
- `wake` : `bool` - Wake up sleeping body

---

#### `GetMass()` → `float`
Returns the mass of the body.

```lua
local mass = rb:GetMass()
print("Mass:", mass)
```

---

### **Particle Emitter Component**

Properties:

#### `Active` : `bool`
Enable/disable particle emission.

```lua
local emitter = self.entity:GetComponent_ParticleEmitter()
emitter.Active = true  -- Start emitting
emitter.Active = false  -- Stop emitting
```

---

#### `ParticlesPerSecond` : `float`
Emission rate.

```lua
emitter.ParticlesPerSecond = 100.0
```

---

#### `Lifetime` : `float`
Particle lifetime in seconds.

```lua
emitter.Lifetime = 1.5
```

---

#### `StartColor` : `Vec4`
Initial particle color.

```lua
emitter.StartColor = Vec4.new(1.0, 1.0, 0.0, 1.0)  -- Yellow
```

---

#### `EndColor` : `Vec4`
Final particle color (interpolated over lifetime).

```lua
emitter.EndColor = Vec4.new(1.0, 0.0, 0.0, 0.0)  -- Red, transparent
```

---

#### `StartSize` : `float`
Initial particle size.

```lua
emitter.StartSize = 0.5
```

---

#### `EndSize` : `float`
Final particle size.

```lua
emitter.EndSize = 0.0  -- Shrink to nothing
```

---

### **Audio Source Component**

Methods:

#### `Play()`
Starts audio playback.

```lua
local audio = self.entity:GetComponent_AudioSource()
audio:Play()
```

---

#### `Stop()`
Stops audio playback.

```lua
audio:Stop()
```

---

#### `Pause()`
Pauses audio playback.

```lua
audio:Pause()
```

---

#### `SetVolume(volume: float)`
Sets volume (0.0 to 1.0).

```lua
audio:SetVolume(0.5)  -- 50% volume
```

**Parameters:**
- `volume` : `float` - Volume level (0.0 = silent, 1.0 = full)

---

#### `SetPitch(pitch: float)`
Sets pitch/speed (1.0 = normal).

```lua
audio:SetPitch(1.5)  -- 1.5x speed, higher pitch
```

**Parameters:**
- `pitch` : `float` - Pitch multiplier (0.5 = slow, 2.0 = fast)

---

#### `SetLooping(looping: bool)`
Enable/disable looping.

```lua
audio:SetLooping(true)  -- Loop forever
```

**Parameters:**
- `looping` : `bool` - Loop enabled

---

#### `IsPlaying()` → `bool`
Returns whether audio is currently playing.

```lua
if audio:IsPlaying() then
    print("Audio is playing")
end
```

---

## 🎮 **Input System**

### **Keyboard Input**

#### `Input.IsKeyPressed(keycode)` → `bool`
Returns true if key is currently pressed.

```lua
if Input.IsKeyPressed(KeyCode.W) then
    print("W key is pressed")
end
```

**Parameters:**
- `keycode` : `KeyCode` - Key to check

**Returns:** `bool` - True if pressed, false otherwise

**Note:** Returns true every frame while key is held down

---

### **Mouse Input**

#### `Input.IsMouseButtonPressed(button)` → `bool`
Returns true if mouse button is currently pressed.

```lua
if Input.IsMouseButtonPressed(0) then  -- Left button
    print("Left mouse button pressed")
end
```

**Parameters:**
- `button` : `int` - Button index (0 = left, 1 = right, 2 = middle)

**Returns:** `bool` - True if pressed, false otherwise

---

### **Mouse Position** *(Coming Soon)*

```lua
-- Future API
local mousePos = Input.GetMousePosition()
print(mousePos.x, mousePos.y)
```

---

## 📐 **Vector Types**

### **Vec2**

2D vector (x, y).

#### Constructor
```lua
local vec = Vec2.new(x, y)
```

#### Properties
```lua
vec.x  -- X component
vec.y  -- Y component
```

#### Example
```lua
local velocity = Vec2.new(5.0, 3.0)
print(velocity.x, velocity.y)  -- 5.0, 3.0

velocity.x = 10.0
velocity.y = -2.0
```

---

### **Vec3**

3D vector (x, y, z).

#### Constructor
```lua
local vec = Vec3.new(x, y, z)
```

#### Properties
```lua
vec.x  -- X component
vec.y  -- Y component
vec.z  -- Z component
```

#### Example
```lua
local position = Vec3.new(1.0, 2.0, 0.0)
print(position.x, position.y, position.z)

position.y = position.y + 1.0  -- Move up
```

---

### **Vec4**

4D vector (x, y, z, w) - Often used for colors (r, g, b, a).

#### Constructor
```lua
local vec = Vec4.new(x, y, z, w)
```

#### Properties
```lua
vec.x  -- X component (or Red)
vec.y  -- Y component (or Green)
vec.z  -- Z component (or Blue)
vec.w  -- W component (or Alpha)
```

#### Example (as color)
```lua
local red = Vec4.new(1.0, 0.0, 0.0, 1.0)
local green = Vec4.new(0.0, 1.0, 0.0, 1.0)
local blue = Vec4.new(0.0, 0.0, 1.0, 1.0)
local transparent = Vec4.new(1.0, 1.0, 1.0, 0.5)  -- 50% alpha
```

---

## ⌨️ **KeyCodes**

### **Letter Keys**
```lua
KeyCode.A, KeyCode.B, KeyCode.C, ..., KeyCode.Z
```

### **Number Keys**
```lua
KeyCode.D0, KeyCode.D1, ..., KeyCode.D9  -- 0-9
```

### **Function Keys**
```lua
KeyCode.F1, KeyCode.F2, ..., KeyCode.F12
```

### **Arrow Keys**
```lua
KeyCode.Up
KeyCode.Down
KeyCode.Left
KeyCode.Right
```

### **Special Keys**
```lua
KeyCode.Space
KeyCode.Enter
KeyCode.Escape
KeyCode.Tab
KeyCode.Backspace
KeyCode.Delete
```

### **Modifier Keys**
```lua
KeyCode.LeftShift, KeyCode.RightShift
KeyCode.LeftControl, KeyCode.RightControl
KeyCode.LeftAlt, KeyCode.RightAlt
KeyCode.LeftSuper, KeyCode.RightSuper  -- Windows/Cmd key
```

### **Example**
```lua
function OnUpdate(self, dt)
    if Input.IsKeyPressed(KeyCode.W) then
        -- Move up
    end
    if Input.IsKeyPressed(KeyCode.S) then
        -- Move down
    end
    if Input.IsKeyPressed(KeyCode.A) then
        -- Move left
    end
    if Input.IsKeyPressed(KeyCode.D) then
        -- Move right
    end
    if Input.IsKeyPressed(KeyCode.Space) then
        -- Jump
    end
end
```

---

## 🔧 **C++ API**

### **Creating Entities**

```cpp
#include <Engine.h>
using namespace Engine;

// Create entity
auto entity = scene->CreateEntity("MyEntity");

// Add Transform (automatic)
auto& transform = entity.GetComponent<TransformComponent>();
transform.Position = { 0.0f, 0.0f, 0.0f };

// Add Sprite Renderer
auto& sprite = entity.AddComponent<SpriteRendererComponent>();
sprite.Color = { 1.0f, 1.0f, 1.0f, 1.0f };

// Add Rigidbody2D
auto& rb = entity.AddComponent<Rigidbody2DComponent>();
rb.Type = Rigidbody2DComponent::BodyType::Dynamic;

// Add Box Collider
auto& collider = entity.AddComponent<BoxCollider2DComponent>();
collider.Size = { 0.5f, 0.5f };
```

---

### **Rendering**

```cpp
#include <Engine/Renderer/Renderer2D.h>
using namespace Engine;

// Begin scene
Renderer2D::BeginScene(camera);

// Draw colored quad
Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f });

// Draw textured quad
Renderer2D::DrawQuad({ 2.0f, 0.0f }, { 1.0f, 1.0f }, texture);

// Draw rotated quad
Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f }, { 1.0f, 1.0f }, 45.0f, color);

// End scene
Renderer2D::EndScene();
```

---

### **Scene Management**

```cpp
#include <Engine/Scene/Scene.h>
#include <Engine/Scene/SceneSerializer.h>
using namespace Engine;

// Create scene
Ref<Scene> scene = CreateRef<Scene>();

// Save scene
SceneSerializer serializer(scene);
serializer.Serialize("assets/scenes/MyScene.yaml");

// Load scene
SceneSerializer serializer(scene);
serializer.Deserialize("assets/scenes/MyScene.yaml");
```

---

### **Audio**

```cpp
#include <Engine/Audio/AudioEngine.h>
#include <Engine/Audio/AudioBuffer.h>
#include <Engine/Audio/AudioSource.h>
using namespace Engine;

// Initialize audio engine (done automatically)
AudioEngine::Init();

// Create audio buffer
auto buffer = AudioBuffer::Create("assets/audio/sound.wav");

// Create audio source
AudioSource source;
source.SetBuffer(buffer);
source.SetPosition({ 0.0f, 0.0f, 0.0f });
source.SetVolume(0.8f);
source.Play();

// Shutdown (done automatically)
AudioEngine::Shutdown();
```

---

### **Particles**

```cpp
#include <Engine/Renderer/ParticleSystem.h>
using namespace Engine;

// Particle properties
ParticleProps props;
props.Position = { 0.0f, 0.0f };
props.Velocity = { 0.0f, 5.0f };
props.VelocityVariation = { 1.0f, 1.0f };
props.ColorBegin = { 1.0f, 1.0f, 0.0f, 1.0f };
props.ColorEnd = { 1.0f, 0.0f, 0.0f, 0.0f };
props.SizeBegin = 0.5f;
props.SizeEnd = 0.0f;
props.LifeTime = 1.0f;

// Emit particle
particleSystem.Emit(props);

// Update particles
particleSystem.OnUpdate(deltaTime);

// Render particles
particleSystem.OnRender(camera);
```

---

### **Input (C++)**

```cpp
#include <Engine/Core/Input.h>
using namespace Engine;

// Keyboard
if (Input::IsKeyPressed(KeyCode::W)) {
    // Move up
}

// Mouse button
if (Input::IsMouseButtonPressed(MouseButton::Left)) {
    // Left click
}

// Mouse position
auto [x, y] = Input::GetMousePosition();
```

---

### **Logging**

```cpp
#include <Engine/Core/Logger.h>

// Core engine logs (use in engine code)
GE_CORE_TRACE("Trace message");
GE_CORE_INFO("Info message");
GE_CORE_WARN("Warning message");
GE_CORE_ERROR("Error message");

// Client logs (use in game code)
GE_TRACE("Trace: {}", value);
GE_INFO("Info: {0}, {1}", val1, val2);
GE_WARN("Warning");
GE_ERROR("Error");
```

---

## 📊 **Constants**

### **Physics Constants**

```cpp
// Gravity (default)
float gravity = 9.8f;  // m/s²

// Time step
float fixedTimeStep = 1.0f / 60.0f;  // 60 Hz

// Velocity iterations
int velocityIterations = 8;

// Position iterations
int positionIterations = 3;
```

---

## 🎯 **Enumerations**

### **Rigidbody2D Body Types**

```cpp
enum class BodyType {
    Static = 0,     // Doesn't move
    Kinematic = 1,  // Moved by code
    Dynamic = 2     // Full physics
};
```

### **Camera Projection Types**

```cpp
enum class ProjectionType {
    Perspective = 0,
    Orthographic = 1
};
```

---

## 📝 **Type Aliases**

```cpp
// Smart pointers
template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T>
using Ref = std::shared_ptr<T>;

// Helper functions
template<typename T, typename... Args>
constexpr Scope<T> CreateScope(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
constexpr Ref<T> CreateRef(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}
```

---

## 🚀 **Advanced Usage**

### **Custom Components (C++)**

```cpp
// Define in Components.h
struct HealthComponent {
    float Health = 100.0f;
    float MaxHealth = 100.0f;
    
    void TakeDamage(float amount) {
        Health = std::max(0.0f, Health - amount);
    }
    
    bool IsDead() const {
        return Health <= 0.0f;
    }
};

// Use in code
auto& health = entity.AddComponent<HealthComponent>();
health.TakeDamage(25.0f);

if (health.IsDead()) {
    scene->DestroyEntity(entity);
}
```

---

### **Custom Systems (C++)**

```cpp
class HealthSystem {
public:
    void OnUpdate(Ref<Scene> scene, TimeStep ts) {
        // Get all entities with HealthComponent
        auto view = scene->GetRegistry().view<HealthComponent>();
        
        for (auto entity : view) {
            auto& health = view.get<HealthComponent>(entity);
            
            // Process health logic
            if (health.IsDead()) {
                // Handle death
            }
        }
    }
};
```

---

## 📖 **Related Documentation**

- **[USER_GUIDE.md](USER_GUIDE.md)** - Complete user guide
- **[ARCHITECTURE.md](ARCHITECTURE.md)** - Engine architecture
- **[CONTRIBUTING.md](CONTRIBUTING.md)** - How to contribute
- **[README.md](README.md)** - Project overview

---

**API Version:** 1.0.0  
**Last Updated:** January 12, 2026  
**Engine:** 2D Game Engine

