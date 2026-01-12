# ⚡ Quick Reference Guide

**Version:** 1.0.0 - One-page cheat sheet for common tasks

---

## 🚀 **Quick Start**

```bash
# Build
mkdir build && cd build
cmake ..
make -j4

# Run Editor
cd bin
./Editor
```

---

## 🎮 **Editor Shortcuts**

| Shortcut | Action |
|----------|--------|
| `Ctrl+N` | New Scene |
| `Ctrl+O` | Open Scene |
| `Ctrl+S` | Save Scene |
| `Ctrl+Shift+S` | Save Scene As |
| `Delete` | Delete Entity |
| `F` | Focus Entity |
| `Space` | Play/Stop |
| `Esc` | Close/Deselect |

---

## 📦 **Common Components**

### **Transform** (Every entity has one)
```lua
transform.Position = Vec3.new(x, y, z)
transform.Rotation = Vec3.new(rx, ry, rz)
transform.Scale = Vec3.new(sx, sy, sz)
```

### **Sprite Renderer**
```lua
sprite.Color = Vec4.new(r, g, b, a)
sprite.TilingFactor = 1.0
```

### **Rigidbody2D**
```lua
rb:SetVelocity(Vec2.new(x, y))
rb:ApplyLinearImpulseToCenter(Vec2.new(x, y), true)
rb:ApplyForce(Vec2.new(x, y), true)
```

### **Audio Source**
```lua
audio:Play()
audio:Stop()
audio:SetVolume(0.5)
```

---

## 🎨 **Lua Script Template**

```lua
-- MyScript.lua
local speed = 5.0

function OnCreate(self)
    print("Created")
end

function OnUpdate(self, dt)
    -- Game logic here
end

function OnDestroy(self)
    print("Destroyed")
end
```

---

## 🎮 **Input Examples**

### **Movement (WASD)**
```lua
function OnUpdate(self, dt)
    local rb = self.entity:GetComponent_Rigidbody2D()
    local moveX, moveY = 0, 0
    
    if Input.IsKeyPressed(KeyCode.W) then moveY = 1 end
    if Input.IsKeyPressed(KeyCode.S) then moveY = -1 end
    if Input.IsKeyPressed(KeyCode.A) then moveX = -1 end
    if Input.IsKeyPressed(KeyCode.D) then moveX = 1 end
    
    rb:SetVelocity(Vec2.new(moveX * speed, moveY * speed))
end
```

### **Jump**
```lua
if Input.IsKeyPressed(KeyCode.Space) then
    rb:ApplyLinearImpulseToCenter(Vec2.new(0, 5), true)
end
```

---

## 🎨 **Common Colors**

```lua
-- Basic colors (R, G, B, A)
local red = Vec4.new(1, 0, 0, 1)
local green = Vec4.new(0, 1, 0, 1)
local blue = Vec4.new(0, 0, 1, 1)
local white = Vec4.new(1, 1, 1, 1)
local black = Vec4.new(0, 0, 0, 1)
local yellow = Vec4.new(1, 1, 0, 1)
local cyan = Vec4.new(0, 1, 1, 1)
local magenta = Vec4.new(1, 0, 1, 1)

-- Transparent
local transparent = Vec4.new(1, 1, 1, 0.5)
```

---

## ⚙️ **Physics Settings**

### **Static Body** (Walls, Ground)
```
Type: Static
```

### **Dynamic Body** (Player, Enemies)
```
Type: Dynamic
Fixed Rotation: ✓
Gravity Scale: 1.0 (or 0 for top-down)
```

### **Common Collider Sizes**
```
Player: Circle, Radius 0.4
Projectile: Circle, Radius 0.1
Platform: Box, Size (width, 0.5)
```

---

## ✨ **Particle Presets**

### **Fire**
```
ParticlesPerSecond: 100
Lifetime: 1.0
StartVelocity: (0, 5)
StartColor: (1, 0.5, 0, 1)  Orange
EndColor: (1, 0, 0, 0)      Red, transparent
```

### **Smoke**
```
ParticlesPerSecond: 50
Lifetime: 2.0
StartVelocity: (0, 3)
StartColor: (0.5, 0.5, 0.5, 0.8)
EndColor: (0.2, 0.2, 0.2, 0)
```

### **Explosion**
```
ParticlesPerSecond: 0  (manual trigger)
Lifetime: 0.5
VelocityVariation: (10, 10)
StartColor: (1, 1, 0, 1)  Yellow
EndColor: (1, 0, 0, 0)    Red, transparent
```

---

## 🔊 **Audio Quick Setup**

1. Add **AudioSource** component
2. Set **Buffer Path**: `assets/audio/sound.wav`
3. Set **Volume**: `0.7`
4. Set **Looping**: ✓ (for music) or ✗ (for SFX)
5. **Play On Start**: ✓ or control via script

```lua
-- In script
audio:Play()
audio:Stop()
audio:SetVolume(0.5)
```

---

## 🎯 **Common Patterns**

### **Timer**
```lua
local timer = 0.0
local interval = 1.0

function OnUpdate(self, dt)
    timer = timer + dt
    if timer >= interval then
        print("1 second passed")
        timer = 0
    end
end
```

### **Toggle**
```lua
local enabled = false

function OnUpdate(self, dt)
    if Input.IsKeyPressed(KeyCode.E) then
        enabled = not enabled
        print("Enabled:", enabled)
    end
end
```

### **Cooldown**
```lua
local cooldown = 0.0
local cooldownTime = 0.5

function OnUpdate(self, dt)
    if cooldown > 0 then
        cooldown = cooldown - dt
    end
    
    if Input.IsKeyPressed(KeyCode.E) and cooldown <= 0 then
        print("Action!")
        cooldown = cooldownTime
    end
end
```

### **Distance Check**
```lua
local targetPos = Vec3.new(5, 0, 0)
local transform = self.entity:GetComponent_Transform()
local pos = transform.Position

local dx = targetPos.x - pos.x
local dy = targetPos.y - pos.y
local distance = math.sqrt(dx*dx + dy*dy)

if distance < 2.0 then
    print("Close to target")
end
```

---

## 📐 **Math Functions**

```lua
-- Trigonometry
math.sin(angle)
math.cos(angle)
math.tan(angle)
math.atan2(y, x)  -- Angle from (0,0) to (x,y)

-- Basic
math.abs(x)       -- Absolute value
math.sqrt(x)      -- Square root
math.pow(x, y)    -- x^y
math.floor(x)     -- Round down
math.ceil(x)      -- Round up

-- Random
math.random()       -- 0 to 1
math.random(10)     -- 1 to 10
math.random(5, 10)  -- 5 to 10

-- Constants
math.pi           -- 3.14159...
```

---

## 🎮 **Complete Game Examples**

### **Platformer Player**
```lua
local speed = 5.0
local jumpForce = 5.0
local canJump = true

function OnCreate(self)
end

function OnUpdate(self, dt)
    local rb = self.entity:GetComponent_Rigidbody2D()
    local vel = rb:GetVelocity()
    
    -- Horizontal movement
    local moveX = 0
    if Input.IsKeyPressed(KeyCode.A) then moveX = -speed end
    if Input.IsKeyPressed(KeyCode.D) then moveX = speed end
    rb:SetVelocity(Vec2.new(moveX, vel.y))
    
    -- Jump
    if Input.IsKeyPressed(KeyCode.Space) and canJump then
        rb:ApplyLinearImpulseToCenter(Vec2.new(0, jumpForce), true)
        canJump = false
    end
end

function OnCollisionEnter(self, other)
    canJump = true
end

function OnDestroy(self)
end
```

### **Top-Down Player**
```lua
local speed = 5.0

function OnCreate(self)
end

function OnUpdate(self, dt)
    local rb = self.entity:GetComponent_Rigidbody2D()
    local moveX, moveY = 0, 0
    
    if Input.IsKeyPressed(KeyCode.W) then moveY = speed end
    if Input.IsKeyPressed(KeyCode.S) then moveY = -speed end
    if Input.IsKeyPressed(KeyCode.A) then moveX = -speed end
    if Input.IsKeyPressed(KeyCode.D) then moveX = speed end
    
    rb:SetVelocity(Vec2.new(moveX, moveY))
end

function OnDestroy(self)
end
```

### **Chase AI**
```lua
local speed = 3.0
local targetPos = Vec3.new(0, 0, 0)  -- Player position

function OnCreate(self)
end

function OnUpdate(self, dt)
    local transform = self.entity:GetComponent_Transform()
    local rb = self.entity:GetComponent_Rigidbody2D()
    local pos = transform.Position
    
    -- Direction to target
    local dx = targetPos.x - pos.x
    local dy = targetPos.y - pos.y
    local distance = math.sqrt(dx*dx + dy*dy)
    
    if distance > 0.1 then
        -- Normalize and move
        dx = dx / distance
        dy = dy / distance
        rb:SetVelocity(Vec2.new(dx * speed, dy * speed))
    end
end

function OnDestroy(self)
end
```

---

## 🐛 **Quick Troubleshooting**

| Problem | Solution |
|---------|----------|
| Black screen | Add entities, check camera position |
| Script not running | Check Console for errors, verify path |
| No physics | Add Rigidbody2D + Collider |
| No audio | Check file path, add AudioListener |
| Low FPS | Check Profiler, reduce particles |
| Can't move | Check body type (Dynamic, not Static) |

---

## 📊 **Performance Targets**

- **FPS:** 60+ (16ms frame time)
- **Entities:** < 1000
- **Particles:** < 500 active
- **Draw Calls:** < 50
- **Physics Bodies:** < 200 dynamic

---

## 📚 **Full Documentation**

- **[USER_GUIDE.md](USER_GUIDE.md)** - Complete user guide
- **[API_REFERENCE.md](API_REFERENCE.md)** - Full API documentation
- **[README.md](README.md)** - Project overview
- **[GAME_SHOOTER_PLAN.md](GAME_SHOOTER_PLAN.md)** - Example game

---

**Quick Reference v1.0.0** | **Print this page for quick access!**

