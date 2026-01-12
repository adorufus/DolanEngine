# 🔫 Building the Top-Down Shooter in Editor

**Game:** Space Survivor  
**Time to Build:** 30-60 minutes  

---

## 📋 **Prerequisites**

Before starting, make sure you have:
- ✅ Engine built successfully (`build/bin/Editor`)
- ✅ All Lua scripts in `Sandbox/assets/scripts/`
- ✅ Editor running

---

## 🎬 **Step-by-Step Build Guide**

### **Step 1: Start the Editor**

```bash
cd build/bin
./Editor
```

---

### **Step 2: Create New Scene**

1. **File → New Scene**
2. The scene will have a default camera and entities
3. Delete any default entities (right-click → Delete)

---

### **Step 3: Create the Player**

1. **Entity → Create Empty** or right-click in hierarchy
2. Rename to "Player"
3. **Add Components:**
   - **Transform**:
     - Position: `(0, 0, 0)`
     - Rotation: `(0, 0, 0)`
     - Scale: `(0.5, 0.5, 1.0)`
   
   - **SpriteRenderer**:
     - Color: Red `(1.0, 0.0, 0.0, 1.0)`
     - (No texture for now, just colored square)
   
   - **Rigidbody2D**:
     - Type: `Dynamic`
     - Fixed Rotation: `✓` (checked)
     - Gravity Scale: `0.0` (no gravity for top-down)
   
   - **CircleCollider2D**:
     - Radius: `0.4`
     - Offset: `(0, 0)`
   
   - **Script**:
     - Script Path: `assets/scripts/ShooterPlayerController.lua`
   
   - **ParticleEmitter** (optional trail effect):
     - Particles Per Second: `50`
     - Lifetime: `0.5`
     - Start Velocity: `(0, -2)` (trail behind)
     - Start Color: Red `(1, 0.2, 0.2, 0.5)`
   
   - **AudioSource** (for shoot sound):
     - Buffer Path: `assets/audio/shoot.wav` (create later)
     - Volume: `0.7`
     - Looping: `✗`

---

### **Step 4: Setup Camera**

1. Find or create "Camera" entity
2. **CameraComponent**:
   - Projection Type: `Orthographic`
   - Orthographic Size: `10.0`
   - Fixed Aspect Ratio: `✓`
   
3. **CameraController**:
   - Follow Mode: `Smooth`
   - Follow Speed: `5.0`
   - Target: Link to Player entity (if possible, or set in script)

---

### **Step 5: Create Game Manager**

1. **Entity → Create Empty**
2. Rename to "GameManager"
3. **Add Components:**
   - **Transform**: (position doesn't matter)
   - **Script**:
     - Script Path: `assets/scripts/GameManager.lua`

---

### **Step 6: Create Enemy Spawner**

1. **Entity → Create Empty**
2. Rename to "EnemySpawner"
3. **Add Components:**
   - **Transform**: Position `(0, 0, 0)`
   - **Script**:
     - Script Path: `assets/scripts/EnemySpawner.lua`

---

### **Step 7: Create Enemy Prefab**

1. **Entity → Create Empty**
2. Rename to "Enemy"
3. **Add Components:**
   - **Transform**:
     - Position: `(5, 5, 0)` (off to side initially)
     - Scale: `(0.4, 0.4, 1.0)`
   
   - **SpriteRenderer**:
     - Color: Green `(0.0, 1.0, 0.0, 1.0)`
   
   - **Rigidbody2D**:
     - Type: `Dynamic`
     - Fixed Rotation: `✓`
     - Gravity Scale: `0.0`
   
   - **CircleCollider2D**:
     - Radius: `0.3`
   
   - **Script**:
     - Script Path: `assets/scripts/ShooterEnemyAI.lua`
   
   - **ParticleEmitter** (death explosion):
     - Particles Per Second: `0` (burst mode, triggered on death)
     - Lifetime: `0.3`

---

### **Step 8: Create Projectile Prefab**

1. **Entity → Create Empty**
2. Rename to "Projectile"
3. **Add Components:**
   - **Transform**:
     - Position: `(10, 10, 0)` (off-screen initially)
     - Scale: `(0.15, 0.15, 1.0)`
   
   - **SpriteRenderer**:
     - Color: White `(1.0, 1.0, 1.0, 1.0)`
   
   - **Rigidbody2D**:
     - Type: `Dynamic`
     - Fixed Rotation: `✓`
     - Gravity Scale: `0.0`
   
   - **CircleCollider2D**:
     - Radius: `0.1`
   
   - **Script**:
     - Script Path: `assets/scripts/Projectile.lua`
   
   - **ParticleEmitter** (trail):
     - Particles Per Second: `100`
     - Lifetime: `0.2`
     - Start Velocity: `(0, 0)`
     - Start Color: Yellow `(1, 1, 0.5, 0.5)`

---

### **Step 9: Add Arena Boundaries (Optional)**

Create 4 walls to keep entities in bounds:

1. **Entity → Create Empty** x4
2. Rename to "WallTop", "WallBottom", "WallLeft", "WallRight"
3. For each wall:
   - **Transform**:
     - Top: Position `(0, 11, 0)`, Scale `(24, 1, 1)`
     - Bottom: Position `(0, -11, 0)`, Scale `(24, 1, 1)`
     - Left: Position `(-11, 0, 0)`, Scale `(1, 24, 1)`
     - Right: Position `(11, 0, 0)`, Scale `(1, 24, 1)`
   
   - **SpriteRenderer**:
     - Color: Dark Gray `(0.2, 0.2, 0.2, 1.0)`
   
   - **Rigidbody2D**:
     - Type: `Static`
   
   - **BoxCollider2D**:
     - Size: `(0.5, 0.5)` (will scale with transform)

---

### **Step 10: Add Audio (Optional)**

1. **Entity → Create Empty**
2. Rename to "BackgroundMusic"
3. **Add Components:**
   - **Transform**: (doesn't matter)
   - **AudioSource**:
     - Buffer Path: `assets/audio/gameplay_music.ogg`
     - Volume: `0.3`
     - Looping: `✓` (checked)
   
4. **Entity → Create Empty**
5. Rename to "AudioListener"
6. **Add Components:**
   - **Transform**: (follow player)
   - **AudioListener** component

---

### **Step 11: Save the Scene**

1. **File → Save Scene As**
2. Save to: `assets/scenes/TopDownShooter.yaml`
3. ✅ Scene is now persistent!

---

## 🎮 **Testing the Game**

### **In Editor:**

1. Click **▶ Play** button in viewport
2. **Controls:**
   - `WASD` - Move player
   - `E` - Shoot (placeholder for mouse button)
   - `Space` - Dash
   - `Escape` - Pause/Resume
   - `Enter` - Start game (from menu)

3. **Check Console Panel** for debug messages:
   - Player movement
   - Shooting events
   - Enemy AI behavior
   - Wave progression

4. Click **■ Stop** to return to editing

### **What You Should See:**

- ✅ Player (red circle) moves with WASD
- ✅ Player rotates toward movement direction
- ✅ Enemies (green circles) chase player
- ✅ Console logs game events
- ✅ Particles trail behind entities
- ⚠️ **Note:** Shooting/spawning needs engine support for runtime entity creation

---

## 📊 **Monitoring Performance**

1. **View → Profiler**
   - Check FPS (should be 60+)
   - Monitor frame time
   
2. **View → Stats**
   - Check draw calls
   - Monitor quad count

---

## 🐛 **Known Limitations**

Current limitations that need engine features:

### **1. Runtime Entity Spawning**
**Issue:** Can't spawn projectiles/enemies from Lua  
**Workaround:** Create entity pools in editor, activate/deactivate  
**Future:** Add `Scene:CreateEntity()` Lua API

### **2. Mouse Input**
**Issue:** No mouse position API in Lua  
**Workaround:** Use `E` key for shooting, aim in movement direction  
**Future:** Add `Input.GetMousePosition()` API

### **3. Entity References**
**Issue:** Can't get references to other entities from Lua  
**Workaround:** Hard-code positions (player at origin)  
**Future:** Add `Scene:FindEntityByName()` API

### **4. Collision Callbacks**
**Issue:** `OnCollisionEnter` not yet wired up  
**Workaround:** Manual distance checks in scripts  
**Future:** Wire up Box2D collision callbacks to Lua

### **5. Entity Destruction**
**Issue:** Can't destroy entities from Lua  
**Workaround:** Move off-screen  
**Future:** Add `entity:Destroy()` API

---

## 🚀 **Next Steps**

### **Phase 1: Make it Playable** (Current)
- ✅ Core scripts written
- ✅ Scene built in editor
- ⏳ Test and iterate

### **Phase 2: Add Missing Engine Features**
- [ ] Runtime entity spawning API
- [ ] Mouse input API
- [ ] Entity reference API
- [ ] Collision callbacks to Lua
- [ ] Entity destruction API

### **Phase 3: Polish**
- [ ] Add proper sprites/textures
- [ ] Add sound effects
- [ ] Add particle effects
- [ ] Balance gameplay
- [ ] Add UI (health, score)
- [ ] Add menus

---

## 💡 **Tips**

1. **Use Asset Browser** to navigate scripts easily
2. **Use Console** to debug Lua behavior
3. **Use Profiler** to monitor performance
4. **Save Often** with `Ctrl+S`
5. **Iterate Quickly** with Play/Stop buttons

---

## 📝 **What We've Built**

✅ **Complete Game Design** (GAME_SHOOTER_PLAN.md)  
✅ **5 Lua Scripts** (Player, Enemy, Projectile, Manager, Spawner)  
✅ **Scene Setup Guide** (This document)  
✅ **Entity Prefabs** (Player, Enemy, Projectile)  
✅ **Game Loop** (Waves, scoring, game states)  

**Status:** 🟡 **PLAYABLE WITH LIMITATIONS**

The core game loop works! Just needs engine features for full functionality.

---

**🎮 Go build it and let's iterate!** 🚀

