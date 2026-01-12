# 🔫 Top-Down Shooter - Game Design Document

**Game Name:** Space Survivor  
**Genre:** Top-Down Shooter / Bullet Hell  
**Engine:** Custom 2D Game Engine v1.0.0  
**Development Time:** 2-3 weeks  

---

## 🎯 **Game Overview**

A fast-paced top-down shooter where the player defends against waves of enemies. Shoot, dodge, survive!

### **Core Gameplay Loop**
1. Player spawns in the center
2. Enemies spawn from edges
3. Player shoots enemies
4. Enemies move toward player
5. Score increases with kills
6. Waves get progressively harder
7. Game over when player health reaches 0

---

## 🎮 **Controls**

| Input | Action |
|-------|--------|
| **WASD** | Move player |
| **Mouse** | Aim direction |
| **Left Click** | Shoot |
| **Space** | Dash (cooldown) |
| **ESC** | Pause menu |

---

## 👤 **Player Mechanics**

### **Stats**
- Health: 100
- Speed: 5.0 units/sec
- Fire Rate: 0.2 seconds
- Dash Speed: 15.0 units
- Dash Cooldown: 2.0 seconds

### **Abilities**
- **Shooting:** Fires projectiles toward mouse cursor
- **Dash:** Quick dodge with invincibility frames
- **Auto-aim assist:** Slight aim correction (optional)

### **Visuals**
- Red circle sprite (player ship)
- Particle trail
- Muzzle flash on shoot
- Shield effect when dashing

---

## 👾 **Enemy Types**

### **1. Basic Enemy (Chaser)**
- Health: 20
- Speed: 3.0
- Damage: 10
- AI: Move directly toward player
- Color: Green

### **2. Fast Enemy (Rusher)**
- Health: 10
- Speed: 6.0
- Damage: 15
- AI: Dash toward player periodically
- Color: Yellow

### **3. Shooter Enemy (Turret)**
- Health: 30
- Speed: 0.5
- Damage: 5 per shot
- AI: Keep distance, shoot at player
- Color: Blue

### **4. Tank Enemy (Boss)**
- Health: 200
- Speed: 1.5
- Damage: 30
- AI: Slow movement, high health
- Color: Red (large)

---

## 💥 **Projectile System**

### **Player Projectile**
- Speed: 15.0 units/sec
- Damage: 25
- Lifetime: 3 seconds
- Visuals: White circle with trail

### **Enemy Projectile**
- Speed: 8.0 units/sec
- Damage: 5
- Lifetime: 5 seconds
- Visuals: Red circle

### **Collision**
- Projectiles destroy on impact
- Particle explosion on hit
- Sound effect on impact

---

## 📊 **Wave System**

### **Wave Progression**
```lua
Wave 1:  5 Basic Enemies
Wave 2:  8 Basic Enemies
Wave 3:  5 Basic + 3 Fast
Wave 4:  10 Basic + 5 Fast
Wave 5:  8 Basic + 5 Fast + 2 Shooters
Wave 10: 1 Tank (Boss)
Wave 11+: Scaling difficulty
```

### **Spawn Logic**
- Enemies spawn at random positions on screen edge
- Spawn rate increases with wave number
- Boss waves every 10 waves
- 3 second break between waves

---

## 🎨 **Visual Effects**

### **Particles**
- ✨ Muzzle flash (player/enemy shoot)
- 💥 Explosion (death)
- 🌟 Trail (projectiles)
- 🛡️ Dash effect (player invincible)
- 💔 Damage indicator (hit player)

### **Screen Effects**
- Camera shake on player hit
- Camera shake on enemy death
- Slight slow-mo on boss death

---

## 🔊 **Audio**

### **Sound Effects**
- `shoot.wav` - Player fires
- `enemy_shoot.wav` - Enemy fires
- `explosion.wav` - Enemy dies
- `hit.wav` - Player hit
- `dash.wav` - Player dash
- `powerup.wav` - Collect item

### **Music**
- `gameplay_music.ogg` - Loop during game
- `boss_music.ogg` - During boss waves

---

## 📈 **Scoring System**

| Action | Points |
|--------|--------|
| Kill Basic Enemy | 10 |
| Kill Fast Enemy | 20 |
| Kill Shooter | 30 |
| Kill Tank | 100 |
| Survive Wave | Wave # × 50 |
| Perfect Wave (no damage) | 200 bonus |

### **High Score**
- Save best score to file
- Display on game over screen

---

## 🏗️ **Technical Implementation**

### **Required Scripts**
1. `PlayerController.lua` - Player movement, shooting, dash
2. `EnemyAI.lua` - Enemy behavior (chaser, rusher, shooter)
3. `Projectile.lua` - Bullet movement and collision
4. `GameManager.lua` - Wave system, scoring, game state
5. `EnemySpawner.lua` - Spawn enemies at positions

### **Required Components**
- TransformComponent (position, rotation)
- SpriteRendererComponent (visual)
- Rigidbody2DComponent (physics)
- CircleCollider2DComponent (collision)
- ScriptComponent (behavior)
- ParticleEmitterComponent (effects)
- AudioSourceComponent (sounds)

### **Scene Structure**
```
TopDownShooter Scene
├── Player
│   ├── Transform
│   ├── SpriteRenderer (red circle)
│   ├── Rigidbody2D (dynamic)
│   ├── CircleCollider2D
│   ├── ScriptComponent (PlayerController.lua)
│   ├── ParticleEmitter (trail)
│   └── AudioSource (shoot sound)
│
├── GameManager
│   └── ScriptComponent (GameManager.lua)
│
├── EnemySpawner
│   └── ScriptComponent (EnemySpawner.lua)
│
├── Camera
│   ├── CameraComponent
│   └── CameraControllerComponent (follow player)
│
└── Audio
    ├── MusicSource (gameplay music)
    └── AudioListener
```

---

## 🎯 **Development Phases**

### **Phase 1: Core Mechanics (Week 1)**
- [x] Player movement
- [ ] Player shooting
- [ ] Mouse aiming
- [ ] Projectile system
- [ ] Basic enemy AI
- [ ] Collision detection
- [ ] Health/damage system

### **Phase 2: Game Systems (Week 2)**
- [ ] Wave spawning
- [ ] Score tracking
- [ ] Enemy variety (3 types)
- [ ] Dash mechanic
- [ ] Particle effects
- [ ] Audio integration

### **Phase 3: Polish (Week 3)**
- [ ] Boss enemy
- [ ] UI elements (health, score)
- [ ] Game over screen
- [ ] Main menu
- [ ] High score save/load
- [ ] Balance tuning
- [ ] Visual polish

---

## 🧪 **Testing Checklist**

- [ ] Player movement feels responsive
- [ ] Shooting is satisfying (audio + particles)
- [ ] Enemies are challenging but fair
- [ ] Waves scale properly
- [ ] No performance issues with many entities
- [ ] Game over state works
- [ ] Score persists correctly
- [ ] Audio levels are balanced

---

## 📊 **Performance Targets**

- **Target FPS:** 60 (16.67ms frame time)
- **Max Entities:** 200 simultaneous
- **Max Particles:** 500 active
- **Memory:** <200 MB

---

## 🚀 **Future Features (Post-Launch)**

- Power-ups (speed boost, rapid fire, shield)
- Multiple weapons (laser, shotgun, rockets)
- Player upgrades between waves
- Different arenas/levels
- Leaderboard integration
- Achievements
- Multiplayer co-op

---

**Let's build this game and stress-test the engine!** 🎮🔫

