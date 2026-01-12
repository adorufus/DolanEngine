# 🎮 Game Engine - Complete User Guide

**Version:** 1.0.0  
**Last Updated:** January 12, 2026  
**Engine Type:** 2D Game Engine with C++ and Lua

---

## 📑 **Table of Contents**

1. [Getting Started](#getting-started)
2. [Editor Guide](#editor-guide)
3. [Creating Your First Game](#creating-your-first-game)
4. [Component Reference](#component-reference)
5. [Lua Scripting Guide](#lua-scripting-guide)
6. [Physics System](#physics-system)
7. [Animation System](#animation-system)
8. [Particle System](#particle-system)
9. [Audio System](#audio-system)
10. [Scene Management](#scene-management)
11. [Performance Optimization](#performance-optimization)
12. [Troubleshooting](#troubleshooting)

---

## 🚀 **Getting Started**

### **Installation & Build**

#### **Prerequisites**
- **CMake** 3.10 or higher
- **C++17** compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- **OpenGL** 3.3+ compatible GPU
- **Git** for cloning dependencies

#### **macOS Installation**
```bash
# Install CMake
brew install cmake

# Clone the repository
git clone https://github.com/yourusername/GameEngine.git
cd GameEngine

# Build the engine
mkdir build && cd build
cmake ..
make -j4

# Run the Editor
cd bin
./Editor
```

#### **Linux Installation**
```bash
# Install dependencies
sudo apt-get update
sudo apt-get install cmake build-essential libgl1-mesa-dev

# Clone and build
git clone https://github.com/yourusername/GameEngine.git
cd GameEngine
mkdir build && cd build
cmake ..
make -j4

# Run the Editor
cd bin
./Editor
```

#### **Windows Installation**
```bash
# Clone the repository
git clone https://github.com/yourusername/GameEngine.git
cd GameEngine

# Generate Visual Studio project
mkdir build
cd build
cmake -G "Visual Studio 16 2019" ..

# Build (or open .sln file in Visual Studio)
cmake --build . --config Release

# Run the Editor
cd bin\Release
Editor.exe
```

### **Project Structure**

```
GameEngine/
├── Engine/              # Core engine library
│   ├── include/        # Public API headers
│   └── src/            # Engine implementation
├── Editor/             # Editor application
├── Sandbox/            # Demo application
├── Vendor/             # Third-party libraries
│   ├── box2d/         # Physics engine
│   ├── entt/          # Entity Component System
│   ├── glfw/          # Windowing
│   ├── imgui/         # Editor UI
│   ├── lua/           # Scripting language
│   └── sol2/          # Lua C++ binding
└── build/              # Build output
    ├── bin/           # Executables and assets
    └── lib/           # Static libraries
```

### **First Run**

After building, run the Editor:

```bash
cd build/bin
./Editor
```

You should see:
- ✅ Editor window opens
- ✅ Scene Hierarchy panel on the left
- ✅ Properties panel on the right
- ✅ Viewport in the center
- ✅ Console at the bottom

---

## 🖥️ **Editor Guide**

### **Editor Layout**

```
┌─────────────────────────────────────────────────────────┐
│  File  Edit  View  Entity  Help                         │
├─────────────┬──────────────────────────┬────────────────┤
│             │                          │                │
│   Scene     │      Viewport            │   Properties   │
│  Hierarchy  │   (Game View)            │     Panel      │
│             │                          │                │
│  - Camera   │  ┌──────────────────┐   │  Transform     │
│  - Player   │  │                  │   │  - Position    │
│  - Enemy    │  │   [Game Scene]   │   │  - Rotation    │
│             │  │                  │   │  - Scale       │
│             │  │  ▶ Play ■ Stop  │   │                │
│             │  └──────────────────┘   │  SpriteRenderer│
│             │                          │  - Color       │
├─────────────┴──────────────────────────┴────────────────┤
│  Console                                                 │
│  [INFO] Scene loaded                                    │
│  [WARN] Texture not found                               │
└─────────────────────────────────────────────────────────┘
```

### **Panels Overview**

#### **1. Scene Hierarchy**
- **Purpose:** Tree view of all entities in the scene
- **Features:**
  - Create new entities (right-click)
  - Select entities (click)
  - Delete entities (Delete key)
  - Rename entities (double-click)
  - Hierarchy structure (parent-child relationships)

**Usage:**
```
Right-click in Scene Hierarchy:
  → Create Empty Entity
  → Create Sprite
  → Create Camera
  → Delete Selected
```

#### **2. Properties Panel**
- **Purpose:** Edit selected entity's components
- **Features:**
  - Add components (Add Component button)
  - Remove components (X button)
  - Edit component properties
  - Live updates (changes apply in real-time)

**Common Properties:**
- Transform (Position, Rotation, Scale)
- Sprite Renderer (Color, Texture)
- Rigidbody2D (Physics properties)
- Colliders (Shape, size, offset)
- Scripts (Lua script path)

#### **3. Viewport**
- **Purpose:** Visual representation of the game scene
- **Controls:**
  - **Pan:** Click and drag
  - **Zoom:** Mouse wheel
  - **Play/Stop:** Control game playback
  - **Focus:** F key (focus selected entity)

**Viewport Toolbar:**
```
▶ Play    - Start game simulation
■ Stop    - Stop and reset
⏸ Pause  - Pause simulation
```

#### **4. Console Panel**
- **Purpose:** Log messages and debugging
- **Features:**
  - Color-coded messages (Info, Warning, Error)
  - Filter by type
  - Clear console
  - Copy messages

**Log Levels:**
- 🔵 **[INFO]** - General information
- 🟡 **[WARN]** - Warnings
- 🔴 **[ERROR]** - Errors
- ⚪ **[TRACE]** - Detailed debug info

#### **5. Asset Browser**
- **Purpose:** Navigate project files
- **Features:**
  - Browse assets folder
  - Preview textures
  - Double-click to open scripts
  - Drag-and-drop to assign assets

**Supported Assets:**
- Textures: `.png`, `.jpg`, `.bmp`
- Scripts: `.lua`
- Scenes: `.yaml`
- Audio: `.wav`, `.ogg`, `.mp3`

#### **6. Profiler Panel**
- **Purpose:** Performance monitoring
- **Metrics:**
  - FPS (Frames Per Second)
  - Frame Time (milliseconds)
  - Draw Calls
  - Quad Count
  - Vertex Count

**Usage:**
```
View → Profiler
Monitor real-time performance
Identify bottlenecks
```

### **Menu Bar**

#### **File Menu**
```
File
├── New Scene (Ctrl+N)
├── Open Scene (Ctrl+O)
├── Save Scene (Ctrl+S)
├── Save Scene As (Ctrl+Shift+S)
└── Exit (Alt+F4)
```

#### **Edit Menu**
```
Edit
├── Undo (Ctrl+Z)         [Coming soon]
├── Redo (Ctrl+Y)         [Coming soon]
├── Copy (Ctrl+C)         [Coming soon]
└── Paste (Ctrl+V)        [Coming soon]
```

#### **View Menu**
```
View
├── Scene Hierarchy
├── Properties
├── Console
├── Asset Browser
├── Profiler
└── Renderer Stats
```

#### **Entity Menu**
```
Entity
├── Create Empty
├── Create Sprite
├── Create Camera
├── Duplicate (Ctrl+D)    [Coming soon]
└── Delete (Delete)
```

### **Keyboard Shortcuts**

| Shortcut | Action |
|----------|--------|
| `Ctrl+N` | New Scene |
| `Ctrl+O` | Open Scene |
| `Ctrl+S` | Save Scene |
| `Ctrl+Shift+S` | Save Scene As |
| `Delete` | Delete Selected Entity |
| `F` | Focus Selected Entity |
| `Space` | Play/Stop Simulation |
| `Esc` | Close Modal/Deselect |

---

## 🎮 **Creating Your First Game**

### **Tutorial: Platformer Game**

Let's create a simple platformer with a player, platforms, and gravity.

#### **Step 1: Create a New Scene**

1. Open the Editor
2. **File → New Scene**
3. A default scene with a camera is created

#### **Step 2: Create the Player**

1. **Entity → Create Empty**
2. Rename to "Player"
3. **Add Components:**

**Transform Component:**
```
Position: (0, 0, 0)
Rotation: (0, 0, 0)
Scale: (0.5, 0.5, 1.0)
```

**Sprite Renderer Component:**
```
Color: (0.0, 0.5, 1.0, 1.0)  // Blue
Texture: (empty for colored square)
```

**Rigidbody2D Component:**
```
Type: Dynamic
Fixed Rotation: ✓ (checked)
Gravity Scale: 1.0
Linear Damping: 0.5
```

**Box Collider2D Component:**
```
Size: (0.5, 0.5)
Offset: (0, 0)
Density: 1.0
Friction: 0.3
```

**Script Component:**
```
Script Path: assets/scripts/PlayerController.lua
```

#### **Step 3: Create Player Controller Script**

Create file: `Sandbox/assets/scripts/PlayerController.lua`

```lua
-- PlayerController.lua
local speed = 5.0
local jumpForce = 3.0
local canJump = true

function OnCreate(self)
    print("[Player] Created!")
end

function OnUpdate(self, dt)
    local rb = self.entity:GetComponent_Rigidbody2D()
    local currentVel = rb:GetVelocity()
    
    -- Horizontal movement
    local moveX = 0.0
    if Input.IsKeyPressed(KeyCode.A) then
        moveX = -speed
    elseif Input.IsKeyPressed(KeyCode.D) then
        moveX = speed
    end
    
    -- Set velocity (preserve Y for gravity)
    rb:SetVelocity(Vec2.new(moveX, currentVel.y))
    
    -- Jump
    if Input.IsKeyPressed(KeyCode.Space) and canJump then
        rb:ApplyLinearImpulseToCenter(Vec2.new(0, jumpForce), true)
        canJump = false
    end
end

function OnCollisionEnter(self, other)
    -- Reset jump when touching ground
    canJump = true
end

function OnDestroy(self)
    print("[Player] Destroyed!")
end
```

#### **Step 4: Create Ground Platform**

1. **Entity → Create Empty**
2. Rename to "Ground"
3. **Add Components:**

**Transform:**
```
Position: (0, -3, 0)
Scale: (10, 0.5, 1)
```

**Sprite Renderer:**
```
Color: (0.3, 0.7, 0.3, 1.0)  // Green
```

**Rigidbody2D:**
```
Type: Static  // Important: doesn't move
```

**Box Collider2D:**
```
Size: (0.5, 0.5)
```

#### **Step 5: Setup Camera**

1. Select "Camera" entity (or create one)
2. **Camera Component:**
```
Projection: Orthographic
Orthographic Size: 5.0
Fixed Aspect Ratio: ✓
Primary: ✓
```

3. **Transform:**
```
Position: (0, 0, 10)  // Pull back from scene
```

#### **Step 6: Test Your Game**

1. Click **▶ Play** button
2. Press **A** and **D** to move
3. Press **Space** to jump
4. The player should:
   - ✅ Move left/right
   - ✅ Jump when on ground
   - ✅ Fall with gravity
   - ✅ Collide with platform

5. Click **■ Stop** to return to editing

#### **Step 7: Add More Platforms**

Duplicate the Ground entity (Ctrl+C, Ctrl+V) and create floating platforms:

**Platform 1:**
```
Position: (-2, 0, 0)
Scale: (2, 0.3, 1)
```

**Platform 2:**
```
Position: (2, 1.5, 0)
Scale: (2, 0.3, 1)
```

#### **Step 8: Save Your Scene**

1. **File → Save Scene As**
2. Save to: `assets/scenes/MyFirstGame.yaml`
3. ✅ Scene is now saved!

### **Tutorial: Top-Down Shooter**

For a complete top-down shooter tutorial, see:
- **[GAME_SHOOTER_PLAN.md](GAME_SHOOTER_PLAN.md)** - Game design
- **[SHOOTER_BUILD_GUIDE.md](SHOOTER_BUILD_GUIDE.md)** - Step-by-step guide

---

## 📦 **Component Reference**

### **Transform Component**

Controls entity position, rotation, and scale.

**Properties:**
```cpp
glm::vec3 Position = {0.0f, 0.0f, 0.0f};
glm::vec3 Rotation = {0.0f, 0.0f, 0.0f};  // Euler angles (degrees)
glm::vec3 Scale = {1.0f, 1.0f, 1.0f};
```

**Lua API:**
```lua
local transform = self.entity:GetComponent_Transform()

-- Read
local pos = transform.Position  -- Vec3
local rot = transform.Rotation  -- Vec3
local scale = transform.Scale   -- Vec3

-- Write
transform.Position = Vec3.new(1.0, 2.0, 0.0)
transform.Rotation = Vec3.new(0, 0, 45)  -- 45° rotation
transform.Scale = Vec3.new(2.0, 2.0, 1.0)
```

**Usage:**
- Every entity has a Transform component
- Cannot be removed
- Position in world space
- Rotation in degrees (Z-axis for 2D rotation)

---

### **Sprite Renderer Component**

Renders a colored quad or textured sprite.

**Properties:**
```cpp
glm::vec4 Color = {1.0f, 1.0f, 1.0f, 1.0f};  // RGBA
Ref<Texture2D> Texture = nullptr;
float TilingFactor = 1.0f;
```

**Lua API:**
```lua
local sprite = self.entity:GetComponent_SpriteRenderer()

-- Change color
sprite.Color = Vec4.new(1.0, 0.0, 0.0, 1.0)  -- Red

-- Tiling (for textured sprites)
sprite.TilingFactor = 2.0  -- Repeat texture 2x
```

**Usage:**
- Makes entity visible
- Color tints the texture (or solid color if no texture)
- Alpha channel for transparency (0.0 = invisible, 1.0 = opaque)

---

### **Camera Component**

Defines the viewpoint for rendering.

**Properties:**
```cpp
enum class ProjectionType { Perspective, Orthographic };

ProjectionType Projection = Orthographic;
float OrthographicSize = 10.0f;
float OrthographicNear = -1.0f;
float OrthographicFar = 1.0f;
bool FixedAspectRatio = false;
bool Primary = true;
```

**Usage:**
- Only one primary camera per scene
- Orthographic for 2D games
- OrthographicSize controls zoom (higher = see more)

---

### **Rigidbody2D Component**

Adds physics simulation to an entity.

**Properties:**
```cpp
enum class BodyType { Static, Kinematic, Dynamic };

BodyType Type = Dynamic;
bool FixedRotation = false;
float GravityScale = 1.0f;
float LinearDamping = 0.0f;
float AngularDamping = 0.01f;
```

**Lua API:**
```lua
local rb = self.entity:GetComponent_Rigidbody2D()

-- Get/Set velocity
local vel = rb:GetVelocity()  -- Vec2
rb:SetVelocity(Vec2.new(5.0, 0.0))

-- Apply forces
rb:ApplyForce(Vec2.new(10.0, 0.0), true)
rb:ApplyLinearImpulseToCenter(Vec2.new(0, 5), true)  -- Instant force

-- Apply torque
rb:ApplyTorque(1.5, true)

-- Get mass
local mass = rb:GetMass()
```

**Body Types:**
- **Static:** Doesn't move (platforms, walls)
- **Kinematic:** Controlled by code, not physics (moving platforms)
- **Dynamic:** Full physics simulation (player, enemies, projectiles)

**Parameters:**
- **FixedRotation:** Prevents rotation (good for players)
- **GravityScale:** Multiplier for gravity (0 = no gravity, 2 = double gravity)
- **LinearDamping:** Air resistance for movement (0-1)
- **AngularDamping:** Air resistance for rotation (0-1)

---

### **Box Collider2D Component**

Box-shaped collision volume.

**Properties:**
```cpp
glm::vec2 Size = {0.5f, 0.5f};
glm::vec2 Offset = {0.0f, 0.0f};
float Density = 1.0f;
float Friction = 0.3f;
float Restitution = 0.0f;  // Bounciness
```

**Usage:**
- Requires Rigidbody2D component
- Size is in world units
- Offset from entity center
- Friction: 0 = slippery, 1 = sticky
- Restitution: 0 = no bounce, 1 = perfect bounce

---

### **Circle Collider2D Component**

Circle-shaped collision volume.

**Properties:**
```cpp
float Radius = 0.5f;
glm::vec2 Offset = {0.0f, 0.0f};
float Density = 1.0f;
float Friction = 0.3f;
float Restitution = 0.0f;
```

**Usage:**
- Same as BoxCollider2D but circular
- Good for balls, wheels, characters

---

### **Script Component**

Attaches Lua script behavior to an entity.

**Properties:**
```cpp
std::string ScriptPath = "";
```

**Usage:**
- Enter relative path: `assets/scripts/MyScript.lua`
- Script must define: `OnCreate`, `OnUpdate`, `OnDestroy`
- Hot-reloading supported (edit script while game runs)

---

### **Particle Emitter Component**

Spawns particle effects.

**Properties:**
```cpp
float ParticlesPerSecond = 100.0f;
float Lifetime = 1.0f;
glm::vec2 StartVelocity = {0.0f, 5.0f};
glm::vec2 VelocityVariation = {1.0f, 1.0f};
glm::vec4 StartColor = {1.0f, 1.0f, 1.0f, 1.0f};
glm::vec4 EndColor = {1.0f, 1.0f, 1.0f, 0.0f};
float StartSize = 0.5f;
float EndSize = 0.0f;
bool Active = true;
```

**Usage:**
- Automatically spawns and animates particles
- StartColor → EndColor transition over lifetime
- StartSize → EndSize shrinking
- VelocityVariation adds randomness

**Common Effects:**
- Fire: Orange start, red end, velocity up
- Smoke: Gray start, black end, velocity up
- Sparks: Bright start, fade to transparent
- Trail: Low particles/sec, short lifetime

---

### **Audio Source Component**

Plays spatial audio.

**Properties:**
```cpp
std::string BufferPath = "";
float Volume = 1.0f;
float Pitch = 1.0f;
bool Looping = false;
bool PlayOnStart = false;
bool Spatial = true;
```

**Lua API:**
```lua
local audio = self.entity:GetComponent_AudioSource()

-- Control playback
audio:Play()
audio:Stop()
audio:Pause()

-- Properties
audio:SetVolume(0.5)  -- 0.0 to 1.0
audio:SetPitch(1.5)   -- Speed multiplier
audio:SetLooping(true)

-- Check state
if audio:IsPlaying() then
    print("Audio is playing")
end
```

**Usage:**
- Spatial audio follows entity position
- Non-spatial for background music (Spatial = false)
- BufferPath: `assets/audio/sound.wav`

---

### **Animation Component**

Controls sprite animation.

**Properties:**
```cpp
Ref<AnimationClip> CurrentClip;
bool Playing = true;
bool Looping = true;
float Speed = 1.0f;
```

**Usage:**
- Requires AnimationClip asset
- Automatically updates sprite texture
- Speed multiplier for faster/slower animation

---

## 🎨 **Lua Scripting Guide**

### **Script Structure**

Every Lua script must define these functions:

```lua
-- Called once when entity is created
function OnCreate(self)
    print("Entity created!")
end

-- Called every frame (dt = delta time in seconds)
function OnUpdate(self, dt)
    -- Game logic here
end

-- Called when entity is destroyed
function OnDestroy(self)
    print("Entity destroyed!")
end

-- Called when collision starts (optional)
function OnCollisionEnter(self, other)
    print("Collision with entity: " .. tostring(other))
end

-- Called when collision ends (optional)
function OnCollisionExit(self, other)
    print("Collision ended")
end
```

### **Entity API**

#### **Getting Components**

```lua
-- Get Transform component
local transform = self.entity:GetComponent_Transform()

-- Get SpriteRenderer
local sprite = self.entity:GetComponent_SpriteRenderer()

-- Get Rigidbody2D
local rb = self.entity:GetComponent_Rigidbody2D()

-- Get Script component
local script = self.entity:GetComponent_Script()
```

#### **Component Properties**

```lua
-- Transform
transform.Position = Vec3.new(0, 0, 0)
transform.Rotation = Vec3.new(0, 0, 45)  -- 45° rotation
transform.Scale = Vec3.new(1, 1, 1)

-- Sprite Renderer
sprite.Color = Vec4.new(1, 0, 0, 1)  -- Red
sprite.TilingFactor = 2.0

-- Rigidbody2D
rb:SetVelocity(Vec2.new(5, 0))
rb:ApplyForce(Vec2.new(10, 0), true)
rb:ApplyLinearImpulseToCenter(Vec2.new(0, 5), true)

-- Audio Source
local audio = self.entity:GetComponent_AudioSource()
audio:Play()
audio:Stop()
audio:SetVolume(0.5)
```

### **Input API**

#### **Keyboard Input**

```lua
-- Check if key is currently pressed
if Input.IsKeyPressed(KeyCode.W) then
    print("W is pressed")
end

-- Available KeyCodes:
KeyCode.A, KeyCode.B, ..., KeyCode.Z
KeyCode.Space
KeyCode.Escape
KeyCode.Enter
KeyCode.LeftShift, KeyCode.RightShift
KeyCode.LeftControl, KeyCode.RightControl
KeyCode.Up, KeyCode.Down, KeyCode.Left, KeyCode.Right
```

#### **Mouse Input**

```lua
-- Check mouse button (0 = left, 1 = right, 2 = middle)
if Input.IsMouseButtonPressed(0) then
    print("Left mouse button pressed")
end

-- Get mouse position (coming soon)
-- local mousePos = Input.GetMousePosition()
```

### **Vector Types**

```lua
-- Vec2 (2D vectors)
local vec2 = Vec2.new(1.0, 2.0)
print(vec2.x, vec2.y)

-- Vec3 (3D vectors)
local vec3 = Vec3.new(1.0, 2.0, 3.0)
print(vec3.x, vec3.y, vec3.z)

-- Vec4 (4D vectors / Colors)
local vec4 = Vec4.new(1.0, 0.0, 0.0, 1.0)  -- Red color
print(vec4.x, vec4.y, vec4.z, vec4.w)
```

### **Math Functions**

```lua
-- Lua standard library
local angle = math.sin(time)
local random = math.random()  -- 0 to 1
local randomInt = math.random(1, 10)  -- 1 to 10
local abs = math.abs(-5)  -- 5
local sqrt = math.sqrt(16)  -- 4

-- Common game math
local distance = math.sqrt((x2-x1)^2 + (y2-y1)^2)
local angle = math.atan2(dy, dx)
```

### **Script Examples**

#### **Example 1: Simple Movement**

```lua
local speed = 5.0

function OnCreate(self)
    print("Movement controller ready")
end

function OnUpdate(self, dt)
    local rb = self.entity:GetComponent_Rigidbody2D()
    local moveX = 0.0
    
    if Input.IsKeyPressed(KeyCode.A) then
        moveX = -speed
    elseif Input.IsKeyPressed(KeyCode.D) then
        moveX = speed
    end
    
    rb:SetVelocity(Vec2.new(moveX, rb:GetVelocity().y))
end

function OnDestroy(self)
end
```

#### **Example 2: Rotating Entity**

```lua
local rotationSpeed = 90.0  -- degrees per second

function OnCreate(self)
end

function OnUpdate(self, dt)
    local transform = self.entity:GetComponent_Transform()
    local currentRot = transform.Rotation
    
    -- Rotate around Z axis
    currentRot.z = currentRot.z + rotationSpeed * dt
    transform.Rotation = currentRot
end

function OnDestroy(self)
end
```

#### **Example 3: Color Changing**

```lua
local time = 0.0

function OnCreate(self)
end

function OnUpdate(self, dt)
    time = time + dt
    
    local sprite = self.entity:GetComponent_SpriteRenderer()
    
    -- Pulse red channel with sine wave
    local red = (math.sin(time * 2) + 1) * 0.5
    sprite.Color = Vec4.new(red, 0.5, 1.0, 1.0)
end

function OnDestroy(self)
end
```

#### **Example 4: Patrol AI**

```lua
local speed = 2.0
local leftBound = -5.0
local rightBound = 5.0
local direction = 1  -- 1 = right, -1 = left

function OnCreate(self)
    print("Patrol AI initialized")
end

function OnUpdate(self, dt)
    local transform = self.entity:GetComponent_Transform()
    local rb = self.entity:GetComponent_Rigidbody2D()
    
    -- Move
    rb:SetVelocity(Vec2.new(speed * direction, rb:GetVelocity().y))
    
    -- Check bounds and flip direction
    if transform.Position.x >= rightBound then
        direction = -1
    elseif transform.Position.x <= leftBound then
        direction = 1
    end
end

function OnDestroy(self)
end
```

#### **Example 5: Jump Controller**

```lua
local jumpForce = 5.0
local canJump = true
local groundCheckTimer = 0.0

function OnCreate(self)
end

function OnUpdate(self, dt)
    local rb = self.entity:GetComponent_Rigidbody2D()
    
    -- Ground check (simple timer)
    groundCheckTimer = groundCheckTimer - dt
    if groundCheckTimer <= 0 then
        canJump = true
    end
    
    -- Jump
    if Input.IsKeyPressed(KeyCode.Space) and canJump then
        rb:ApplyLinearImpulseToCenter(Vec2.new(0, jumpForce), true)
        canJump = false
        groundCheckTimer = 0.3  -- 300ms cooldown
    end
end

function OnCollisionEnter(self, other)
    -- Reset jump when hitting something below
    canJump = true
    groundCheckTimer = 0
end

function OnDestroy(self)
end
```

### **Hot-Reloading**

The engine supports hot-reloading of Lua scripts:

1. **Edit script** while game is running
2. **Save file** (Ctrl+S in your text editor)
3. **Script automatically reloads** in the engine
4. **Changes take effect immediately**

**Note:** `OnCreate()` is called again when script reloads.

---

## ⚙️ **Physics System**

The engine uses **Box2D** for 2D physics simulation.

### **Physics Setup**

1. Add **Rigidbody2D** component to entity
2. Add **Collider** component (BoxCollider2D or CircleCollider2D)
3. Configure physics properties
4. Script can interact via Lua API

### **Body Types**

#### **Static**
- Never moves
- Use for: Ground, walls, platforms
- Most performant

```cpp
Rigidbody2D:
  Type: Static
```

#### **Kinematic**
- Moved by code, not physics
- Use for: Moving platforms, doors
- Doesn't respond to collisions

```cpp
Rigidbody2D:
  Type: Kinematic
```

#### **Dynamic**
- Full physics simulation
- Use for: Player, enemies, projectiles
- Affected by gravity and forces

```cpp
Rigidbody2D:
  Type: Dynamic
  GravityScale: 1.0
```

### **Collision Filtering**

Coming soon: Collision layers and masks for selective collision.

### **Physics Best Practices**

1. **Use CircleCollider2D for moving entities** - Smoother than boxes
2. **Set FixedRotation for characters** - Prevents tipping over
3. **Use smaller time step for precision** - Engine default is good
4. **Avoid very fast moving objects** - Use continuous collision detection
5. **Keep physics bodies reasonable size** - 0.5 to 10 units recommended

---

## 🎬 **Animation System**

The engine supports sprite sheet animations.

### **Animation Workflow**

1. Prepare sprite sheet (horizontal strip of frames)
2. Create AnimationClip asset
3. Add Animation component to entity
4. Control playback via properties or script

### **Animation Properties**

```cpp
Playing: true/false     // Is animation playing?
Looping: true/false     // Loop when finished?
Speed: 1.0              // Playback speed multiplier
```

### **Example Usage**

```lua
function OnUpdate(self, dt)
    local anim = self.entity:GetComponent_Animation()
    
    -- Control playback
    if Input.IsKeyPressed(KeyCode.P) then
        anim.Playing = not anim.Playing
    end
    
    -- Speed up animation
    anim.Speed = 2.0  -- 2x speed
end
```

---

## ✨ **Particle System**

Create visual effects with the particle system.

### **Particle Setup**

1. Add **ParticleEmitter** component
2. Configure emitter properties
3. Toggle `Active` to start/stop emission

### **Common Effects**

#### **Fire Effect**
```
ParticlesPerSecond: 100
Lifetime: 1.0
StartVelocity: (0, 5)
VelocityVariation: (2, 1)
StartColor: (1.0, 0.5, 0.0, 1.0)  // Orange
EndColor: (1.0, 0.0, 0.0, 0.0)    // Red, transparent
StartSize: 0.5
EndSize: 0.0
```

#### **Smoke Effect**
```
ParticlesPerSecond: 50
Lifetime: 2.0
StartVelocity: (0, 3)
VelocityVariation: (1, 0.5)
StartColor: (0.5, 0.5, 0.5, 0.8)  // Gray
EndColor: (0.2, 0.2, 0.2, 0.0)    // Dark, transparent
StartSize: 0.3
EndSize: 0.8
```

#### **Explosion Effect (Burst)**
```
ParticlesPerSecond: 0  // Trigger manually
Lifetime: 0.5
StartVelocity: (0, 0)
VelocityVariation: (10, 10)
StartColor: (1.0, 1.0, 0.0, 1.0)  // Yellow
EndColor: (1.0, 0.0, 0.0, 0.0)    // Red, transparent
StartSize: 0.5
EndSize: 0.0
```

### **Lua Control**

```lua
function OnUpdate(self, dt)
    local emitter = self.entity:GetComponent_ParticleEmitter()
    
    -- Toggle emission
    emitter.Active = true
    
    -- Change colors
    emitter.StartColor = Vec4.new(1, 0, 0, 1)
    emitter.EndColor = Vec4.new(0, 0, 1, 0)
    
    -- Burst effect (spawn many particles instantly)
    if Input.IsKeyPressed(KeyCode.E) then
        emitter.ParticlesPerSecond = 1000
        -- Reset after 0.1 second
    end
end
```

---

## 🔊 **Audio System**

The engine uses **OpenAL** for 3D spatial audio.

### **Audio Setup**

1. Add **AudioSource** component to entity
2. Set **BufferPath** to audio file
3. Configure volume, pitch, looping
4. Control playback via properties or script

### **Audio Formats**

Supported: `.wav`, `.ogg`, `.mp3`

Place audio files in: `assets/audio/`

### **Spatial Audio**

When `Spatial = true`:
- Volume decreases with distance from listener
- Panning based on position (left/right)
- 3D positioning

When `Spatial = false`:
- Constant volume (good for music)
- No panning

### **Audio Listener**

- Add **AudioListener** component to camera or player
- Only one listener per scene
- Position determines audio perception point

### **Lua Audio Control**

```lua
function OnUpdate(self, dt)
    local audio = self.entity:GetComponent_AudioSource()
    
    -- Play sound on key press
    if Input.IsKeyPressed(KeyCode.E) then
        audio:Play()
    end
    
    -- Stop sound
    if Input.IsKeyPressed(KeyCode.S) then
        audio:Stop()
    end
    
    -- Adjust volume
    audio:SetVolume(0.5)  -- 50% volume
    
    -- Pitch shift (speed)
    audio:SetPitch(1.5)  -- 1.5x speed, higher pitch
    
    -- Check if playing
    if audio:IsPlaying() then
        print("Audio is playing")
    end
end
```

---

## 💾 **Scene Management**

### **Saving Scenes**

Scenes are saved as **YAML** files.

**Save Scene:**
1. **File → Save Scene** (Ctrl+S)
2. Or **File → Save Scene As** (Ctrl+Shift+S)
3. Choose location: `assets/scenes/MyScene.yaml`

**What's Saved:**
- ✅ All entities and their names
- ✅ All component data
- ✅ Transform, Sprite, Physics, Scripts, etc.
- ✅ Relationships (parent-child)

**Not Saved:**
- ❌ Runtime state (velocity, script variables)
- ❌ Active/playing states

### **Loading Scenes**

**Load Scene:**
1. **File → Open Scene** (Ctrl+O)
2. Navigate to `.yaml` file
3. Scene loads and replaces current scene

### **Scene Structure**

Example scene file:

```yaml
Scene: MyGame
Entities:
  - Entity: 12345678
    TagComponent:
      Tag: Player
    TransformComponent:
      Position: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    SpriteRendererComponent:
      Color: [1, 0, 0, 1]
    Rigidbody2DComponent:
      Type: Dynamic
      FixedRotation: true
      GravityScale: 1.0
    BoxCollider2DComponent:
      Size: [0.5, 0.5]
    ScriptComponent:
      ScriptPath: assets/scripts/PlayerController.lua
```

### **Multiple Scenes**

Currently, the engine loads one scene at a time. To create multiple levels:

1. Create separate scene files: `Level1.yaml`, `Level2.yaml`
2. Load different scenes via File menu
3. (Future: Scene transitions via script API)

---

## ⚡ **Performance Optimization**

### **Profiling**

**View → Profiler** to monitor:
- FPS (Frames Per Second)
- Frame Time (milliseconds)
- Draw Calls
- Quad Count
- Vertex Count

**Target Performance:**
- **FPS:** 60+ (16.67ms frame time)
- **Draw Calls:** < 50 for most 2D games
- **Entities:** 500+ should be fine

### **Optimization Tips**

#### **1. Batch Rendering**
- Engine automatically batches sprites with same texture
- **Keep entities using same textures together**
- Avoid switching textures frequently

#### **2. Particle Systems**
- Limit total particles to < 1000 active
- Use shorter lifetimes
- Reduce ParticlesPerSecond

#### **3. Physics**
- Use Static bodies for non-moving objects
- Limit Dynamic bodies to < 200
- Simplify collision shapes (prefer circles)

#### **4. Scripts**
- Avoid expensive calculations in `OnUpdate()`
- Cache component references in `OnCreate()`
- Use timer-based checks instead of every-frame

Example of **efficient** scripting:

```lua
-- GOOD: Cache component reference
local rb = nil

function OnCreate(self)
    rb = self.entity:GetComponent_Rigidbody2D()  -- Cache once
end

function OnUpdate(self, dt)
    rb:SetVelocity(Vec2.new(5, 0))  -- Use cached reference
end
```

```lua
-- BAD: Get component every frame
function OnUpdate(self, dt)
    local rb = self.entity:GetComponent_Rigidbody2D()  -- Slow!
    rb:SetVelocity(Vec2.new(5, 0))
end
```

#### **5. Textures**
- Use power-of-2 texture sizes (256x256, 512x512, 1024x1024)
- Keep textures reasonably sized (< 2048x2048)
- Use texture atlases for multiple sprites

#### **6. Scene Complexity**
- Keep entity count reasonable (< 1000 entities)
- Disable off-screen entities (future feature)
- Use object pooling for frequently spawned entities

### **Performance Benchmarks**

**Hardware:** Apple M4  
**Resolution:** 1280x720  
**Build:** Release mode

| Scenario | FPS | Frame Time | Draw Calls |
|----------|-----|------------|------------|
| Empty Scene | 600+ | ~1.5ms | 0 |
| 100 Sprites | 60+ | ~16ms | 1-2 |
| 1000 Sprites | 60 | ~16ms | 5-10 |
| 100 Physics Bodies | 60 | ~16ms | 1-2 |
| With Particles (500) | 60 | ~16ms | 10-15 |

---

## 🐛 **Troubleshooting**

### **Build Issues**

#### **CMake not found**
```
Error: cmake: command not found
```
**Solution:**
```bash
# macOS
brew install cmake

# Linux
sudo apt-get install cmake

# Windows
Download from https://cmake.org/download/
```

#### **OpenGL errors on macOS**
```
Error: OpenGL version too old
```
**Solution:**
- macOS supports OpenGL 3.3 to 4.1
- Use `#version 330 core` in shaders
- Engine is configured correctly by default

#### **Missing dependencies**
```
Error: GLFW not found
```
**Solution:**
```bash
# Dependencies are in Vendor/ folder
# Make sure you cloned with submodules:
git submodule update --init --recursive
```

### **Runtime Issues**

#### **Black screen in viewport**
**Possible causes:**
1. No entities in scene
2. Camera not positioned correctly
3. Entities behind camera

**Solution:**
- Add entities to scene
- Check camera position (Z should be positive)
- Check entity Z positions (should be less than camera)

#### **Script not running**
**Possible causes:**
1. Script path incorrect
2. Syntax error in Lua
3. Missing OnCreate/OnUpdate functions

**Solution:**
- Check Console panel for errors
- Verify script path: `assets/scripts/MyScript.lua`
- Ensure OnCreate, OnUpdate, OnDestroy are defined

#### **Physics not working**
**Possible causes:**
1. Missing Rigidbody2D component
2. Missing Collider component
3. Body type is Static

**Solution:**
- Add both Rigidbody2D and Collider
- Set body type to Dynamic for moving objects
- Check GravityScale (set to 0 for top-down games)

#### **Audio not playing**
**Possible causes:**
1. Audio file not found
2. Incorrect file path
3. No AudioListener in scene

**Solution:**
- Place audio in `assets/audio/`
- Use relative path: `assets/audio/sound.wav`
- Add AudioListener component to camera or player

### **Performance Issues**

#### **Low FPS**
**Possible causes:**
1. Too many entities
2. Too many particles
3. Complex physics simulation
4. Debug build (not Release)

**Solution:**
- Use Profiler to identify bottleneck
- Reduce particle counts
- Limit physics bodies
- Build in Release mode: `cmake --build . --config Release`

### **Editor Issues**

#### **Panels not visible**
**Solution:**
- **View → [Panel Name]** to show hidden panels
- Reset layout (coming soon)

#### **Can't select entities**
**Solution:**
- Click in Scene Hierarchy panel
- Or click directly in Viewport

#### **Changes not saving**
**Solution:**
- **File → Save Scene** (Ctrl+S)
- Check Console for save errors

---

## 📖 **Additional Resources**

### **Documentation**
- **README.md** - Project overview
- **ARCHITECTURE.md** - Engine architecture
- **BUILD.md** - Build instructions
- **CONTRIBUTING.md** - How to contribute
- **CHANGELOG.md** - Version history
- **ROADMAP.md** - Future plans

### **Example Projects**
- **Sandbox/** - Demo application with examples
- **GAME_SHOOTER_PLAN.md** - Top-down shooter design
- **SHOOTER_BUILD_GUIDE.md** - Tutorial for building shooter

### **Learning Resources**
- **[Learn OpenGL](https://learnopengl.com/)** - Graphics programming
- **[Lua Programming](https://www.lua.org/manual/5.4/)** - Scripting
- **[Box2D Manual](https://box2d.org/documentation/)** - Physics
- **[Game Programming Patterns](https://gameprogrammingpatterns.com/)** - Design patterns

### **Community**
- **GitHub Issues** - Bug reports and feature requests
- **GitHub Discussions** - Questions and community
- **Pull Requests** - Contributions welcome!

---

## 🎉 **Conclusion**

You now have everything you need to create 2D games with this engine!

**Quick Recap:**
1. ✅ **Build** the engine with CMake
2. ✅ **Run** the Editor
3. ✅ **Create** entities and add components
4. ✅ **Write** Lua scripts for behavior
5. ✅ **Test** with Play/Stop buttons
6. ✅ **Save** your scene
7. ✅ **Build** amazing games!

**Next Steps:**
- Follow the platformer or shooter tutorial
- Experiment with components and scripts
- Create your own game!
- Join the community

---

**Happy Game Development! 🎮🚀**

**Engine Version:** 1.0.0  
**Documentation Updated:** January 12, 2026  
**License:** MIT

