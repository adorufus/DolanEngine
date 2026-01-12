# ❓ Frequently Asked Questions (FAQ)

**Version:** 1.0.0  
**Last Updated:** January 12, 2026

---

## 📑 **Table of Contents**

1. [General Questions](#general-questions)
2. [Installation & Setup](#installation--setup)
3. [Editor Usage](#editor-usage)
4. [Scripting Questions](#scripting-questions)
5. [Physics & Collision](#physics--collision)
6. [Audio & Particles](#audio--particles)
7. [Performance & Optimization](#performance--optimization)
8. [Common Issues](#common-issues)
9. [Development & Contributing](#development--contributing)

---

## 🌟 **General Questions**

### **What is this game engine?**

This is a 2D game engine built with C++17 and OpenGL, designed for creating 2D games like platformers, top-down shooters, puzzle games, and arcade-style games. It includes:
- Visual editor with ImGui
- Lua scripting for game logic
- Physics (Box2D)
- Animation system
- Particle effects
- Spatial audio
- Scene management

**Best for:** Indie developers, students, hobbyists, and anyone wanting to make 2D games.

---

### **Is it free?**

Yes! The engine is **open source** under the **MIT License**, which means:
- ✅ Free to use
- ✅ Free for commercial games
- ✅ Modify as you wish
- ✅ No royalties or fees
- ✅ Credit appreciated but not required

---

### **What platforms are supported?**

**Development (Engine itself):**
- ✅ macOS (OpenGL 3.3-4.1)
- ✅ Linux (OpenGL 3.3+)
- ✅ Windows (OpenGL 3.3+)

**Game Export:**
- Currently games run on the same platforms as development
- Future: Mobile (iOS, Android), Web (WebGL)

---

### **What types of games can I make?**

**Well-Suited For:**
- ✅ Platformers (Mario-style, Celeste-style)
- ✅ Top-down games (shooters, RPGs, roguelikes)
- ✅ Puzzle games (match-3, physics puzzles)
- ✅ Arcade games (Pac-Man, Space Invaders style)
- ✅ Side-scrollers (shoot-em-ups, runners)
- ✅ Physics-based games

**Not Recommended For:**
- ❌ 3D games (2D only)
- ❌ Large open-world games (performance limits)
- ❌ MMORPGs (no networking yet)

---

### **How does it compare to Unity/Godot/GameMaker?**

| Feature | This Engine | Unity | Godot | GameMaker |
|---------|------------|-------|-------|-----------|
| **2D Focus** | ✅ | 🟡 | ✅ | ✅ |
| **Free & Open Source** | ✅ | 🟡 | ✅ | ❌ |
| **C++ Performance** | ✅ | 🟡 | 🟡 | ❌ |
| **Easy Scripting** | ✅ (Lua) | ✅ (C#) | ✅ (GDScript) | ✅ (GML) |
| **Visual Editor** | ✅ | ✅ | ✅ | ✅ |
| **Learning Curve** | Easy | Steep | Medium | Easy |
| **3D Support** | ❌ | ✅ | ✅ | ❌ |
| **Mobile Export** | ⏳ Future | ✅ | ✅ | ✅ |
| **Maturity** | New | Mature | Mature | Mature |

**Choose this engine if:**
- You want to learn game engine architecture
- You prefer lightweight tools
- You like Lua scripting
- You want full source code access
- You're making simple-to-moderate 2D games

---

### **Do I need to know C++?**

**No!** For making games, you only need:
- Basic Lua scripting (easier than C++)
- Understanding of game concepts (entities, components)
- Using the visual editor

**You only need C++ if:**
- Contributing to the engine itself
- Creating custom engine features
- Deep engine modifications

---

### **Can I sell games made with this engine?**

**Yes, absolutely!** The MIT License allows commercial use:
- ✅ Sell your games
- ✅ No royalties
- ✅ No revenue sharing
- ✅ Keep all profits

**Optional:** Credit the engine in your game (appreciated but not required).

---

## 💻 **Installation & Setup**

### **What do I need to install?**

**Required:**
- CMake 3.10+
- C++17 compiler (GCC 7+, Clang 5+, MSVC 2017+)
- Git
- OpenGL 3.3+ compatible GPU

**Automatic (included):**
- All dependencies are in the `Vendor/` folder
- No manual library installation needed!

See **[SETUP.md](SETUP.md)** for detailed instructions.

---

### **Build fails with "CMake not found"**

**Solution:**
```bash
# macOS
brew install cmake

# Linux
sudo apt-get install cmake

# Windows
Download from https://cmake.org/download/
```

---

### **Build fails with OpenGL errors**

**macOS:**
- macOS supports OpenGL 3.3 to 4.1 (Core Profile)
- The engine is already configured correctly
- If errors persist, update macOS

**Linux:**
```bash
# Install OpenGL development files
sudo apt-get install libgl1-mesa-dev
```

**Windows:**
- Update graphics drivers
- Ensure GPU supports OpenGL 3.3+

---

### **How long does the build take?**

**First build:**
- macOS: 2-5 minutes
- Linux: 3-7 minutes
- Windows: 5-10 minutes

**Incremental builds:**
- < 5 seconds (only modified files recompile)

---

### **Where are the executables?**

After building:
```
build/
└── bin/
    ├── Editor        # Editor application
    ├── Sandbox       # Demo application
    └── assets/       # Asset folder
```

Run from `build/bin/` directory:
```bash
cd build/bin
./Editor
```

---

## 🖥️ **Editor Usage**

### **What are the main editor panels?**

1. **Scene Hierarchy** - Tree of entities
2. **Properties** - Edit selected entity
3. **Viewport** - Visual game view
4. **Console** - Logs and messages
5. **Asset Browser** - File navigator
6. **Profiler** - Performance metrics

See **[USER_GUIDE.md](USER_GUIDE.md)** - Editor Guide section.

---

### **How do I create an entity?**

**Method 1:**
- Right-click in Scene Hierarchy
- Select "Create Empty" or "Create Sprite"

**Method 2:**
- Menu: Entity → Create Empty

**Method 3:**
- Shortcut: (Coming soon)

---

### **How do I add components?**

1. Select entity in Scene Hierarchy
2. In Properties panel, click "Add Component"
3. Choose component type
4. Configure properties

**Common components:**
- Transform (automatic)
- Sprite Renderer (visual)
- Rigidbody2D (physics)
- Colliders (collision)
- Script (behavior)

---

### **How do I move entities?**

**In Editor:**
- Select entity
- In Properties, edit Transform Position: `(x, y, z)`

**In Scripts:**
```lua
local transform = self.entity:GetComponent_Transform()
transform.Position = Vec3.new(1, 2, 0)
```

---

### **How do I test my game?**

1. Click **▶ Play** button in viewport
2. Game simulation starts
3. Test gameplay
4. Click **■ Stop** to return to editing

**Note:** Changes during Play mode are not saved!

---

### **How do I save my scene?**

**File → Save Scene** (Ctrl+S)

Or **File → Save Scene As** (Ctrl+Shift+S) for new name.

Scenes are saved as `.yaml` files in `assets/scenes/`.

---

### **Why is my viewport black?**

**Common causes:**

1. **No entities:** Add entities to the scene
2. **Camera too far:** Check camera Position.z (should be positive)
3. **Entities behind camera:** Entities need Position.z < Camera.z
4. **No Sprite Renderer:** Add SpriteRendererComponent to entities

**Solution:**
- Add a sprite entity
- Check camera position: `(0, 0, 10)`
- Check sprite position: `(0, 0, 0)`

---

## 📜 **Scripting Questions**

### **What scripting language is used?**

**Lua 5.4** - A lightweight, easy-to-learn scripting language.

**Why Lua?**
- ✅ Easy to learn (easier than C++)
- ✅ Fast execution
- ✅ Hot-reloading (edit while game runs)
- ✅ Safe (can't crash engine)
- ✅ Industry standard (used in many games)

---

### **Do I need to know Lua?**

Basic Lua is very easy to learn!

**If you know any programming language**, you'll learn Lua in 30 minutes:
- Variables: `local x = 5`
- If statements: `if x > 0 then ... end`
- Functions: `function myFunc() ... end`
- Tables: `local t = {1, 2, 3}`

See **[USER_GUIDE.md](USER_GUIDE.md)** - Lua Scripting Guide.

---

### **What's the basic script structure?**

```lua
-- Called once when created
function OnCreate(self)
    -- Initialize variables
end

-- Called every frame
function OnUpdate(self, dt)
    -- Game logic
end

-- Called when destroyed
function OnDestroy(self)
    -- Cleanup
end
```

**That's it!** Just define these three functions.

---

### **How do I access components in scripts?**

```lua
-- Get Transform
local transform = self.entity:GetComponent_Transform()

-- Get Sprite Renderer
local sprite = self.entity:GetComponent_SpriteRenderer()

-- Get Rigidbody2D
local rb = self.entity:GetComponent_Rigidbody2D()

-- Get Audio Source
local audio = self.entity:GetComponent_AudioSource()
```

---

### **How do I handle input?**

```lua
function OnUpdate(self, dt)
    -- Check if key is pressed
    if Input.IsKeyPressed(KeyCode.W) then
        print("W is pressed")
    end
    
    -- Check mouse button
    if Input.IsMouseButtonPressed(0) then  -- 0 = left
        print("Left click")
    end
end
```

See **[API_REFERENCE.md](API_REFERENCE.md)** for all KeyCodes.

---

### **Can I hot-reload scripts?**

**Yes!** Edit your Lua script and save:
1. Script automatically reloads
2. `OnCreate()` is called again
3. Changes take effect immediately

**No need to restart!**

---

### **How do I debug scripts?**

**Use print statements:**
```lua
print("Debug:", value)
print(string.format("Position: (%.2f, %.2f)", x, y))
```

**Check Console panel:**
- All print() output appears in Console
- Color-coded by level
- Filter by type

---

### **Can I create entities from scripts?**

**Not yet.** This feature is planned for v1.1.

**Current workaround:**
- Create entity pool in editor
- Activate/deactivate entities
- Move on-screen/off-screen

---

## ⚙️ **Physics & Collision**

### **How do I add physics?**

1. Add **Rigidbody2D** component
2. Add **Collider** component (BoxCollider2D or CircleCollider2D)
3. Set body type:
   - **Static** - Doesn't move (walls)
   - **Dynamic** - Full physics (player, enemies)
   - **Kinematic** - Controlled by code

---

### **Why isn't my entity falling?**

**Check:**
1. Body type is **Dynamic** (not Static)
2. Gravity Scale > 0 (default is 1.0)
3. Scene has gravity enabled

**For top-down games:**
- Set Gravity Scale to 0 (no gravity)

---

### **How do I detect collisions?**

In your Lua script:

```lua
function OnCollisionEnter(self, other)
    print("Collision started!")
    -- Handle collision
end

function OnCollisionExit(self, other)
    print("Collision ended!")
end
```

**Requirements:**
- Both entities need Rigidbody2D
- Both entities need Colliders
- At least one must be Dynamic

---

### **How do I make an entity jump?**

```lua
local jumpForce = 5.0

function OnUpdate(self, dt)
    if Input.IsKeyPressed(KeyCode.Space) then
        local rb = self.entity:GetComponent_Rigidbody2D()
        rb:ApplyLinearImpulseToCenter(Vec2.new(0, jumpForce), true)
    end
end
```

**Tips:**
- Use impulse (not force) for instant jump
- Add cooldown to prevent double-jump
- Check if on ground (use collision callbacks)

---

### **How do I move with physics?**

**Method 1: Set velocity directly**
```lua
rb:SetVelocity(Vec2.new(5, 0))  -- Move right
```

**Method 2: Apply force**
```lua
rb:ApplyForce(Vec2.new(10, 0), true)  -- Continuous push
```

**For platformers:** Use SetVelocity for responsive controls.  
**For realistic movement:** Use ApplyForce for acceleration.

---

### **How do I prevent rotation?**

Set **Fixed Rotation** to ✓ (checked) in Rigidbody2D component.

**Or in Lua:**
```lua
-- (Feature coming soon)
```

---

## 🔊 **Audio & Particles**

### **How do I play a sound?**

**In Editor:**
1. Add **AudioSource** component
2. Set **Buffer Path**: `assets/audio/sound.wav`
3. Check **Play On Start** (or control via script)

**In Script:**
```lua
local audio = self.entity:GetComponent_AudioSource()
audio:Play()
```

---

### **What audio formats are supported?**

- ✅ `.wav` (best compatibility)
- ✅ `.ogg` (compressed, good for music)
- ✅ `.mp3` (compressed)

**Recommendation:** Use `.wav` for short SFX, `.ogg` for music.

---

### **How do I create particle effects?**

1. Add **ParticleEmitter** component
2. Configure properties:
   - ParticlesPerSecond
   - Lifetime
   - StartColor / EndColor
   - StartSize / EndSize
3. Set **Active** to ✓

**See presets in [QUICK_REFERENCE.md](QUICK_REFERENCE.md)** - Particle Presets.

---

### **How do I make an explosion effect?**

```
ParticlesPerSecond: 0  (burst mode)
Lifetime: 0.5
VelocityVariation: (10, 10)  (spread in all directions)
StartColor: Yellow (1, 1, 0, 1)
EndColor: Red transparent (1, 0, 0, 0)
```

Trigger from script:
```lua
emitter.ParticlesPerSecond = 1000
-- Reset after short delay
```

---

## ⚡ **Performance & Optimization**

### **What FPS should I expect?**

**Typical performance:**
- Empty scene: 600+ FPS
- 100 sprites: 60+ FPS
- 1000 sprites: 60 FPS
- With physics and particles: 60 FPS

**If below 60 FPS:**
- Check Profiler panel (View → Profiler)
- See optimization tips below

---

### **How do I improve performance?**

**Quick wins:**
1. **Batch rendering** - Use same texture for multiple sprites
2. **Limit particles** - Keep under 500 active particles
3. **Reduce physics** - Use Static bodies where possible
4. **Cache components** - Get components in OnCreate(), not OnUpdate()
5. **Build Release mode** - `cmake --build . --config Release`

See **[USER_GUIDE.md](USER_GUIDE.md)** - Performance Optimization.

---

### **How many entities can I have?**

**Comfortable limits:**
- **Entities:** 500-1000
- **Dynamic physics bodies:** 100-200
- **Active particles:** 500
- **Draw calls:** < 50

**Beyond these limits:**
- Performance may degrade
- Test on target hardware
- Profile with Profiler panel

---

## 🐛 **Common Issues**

### **"Script not found" error**

**Cause:** Script path is incorrect.

**Solution:**
- Use relative path from project root
- Example: `assets/scripts/PlayerController.lua`
- Check file exists at that location
- Check spelling and case (case-sensitive on Linux/macOS)

---

### **Physics not working**

**Checklist:**
- [ ] Entity has Rigidbody2D component
- [ ] Entity has Collider component (BoxCollider2D or CircleCollider2D)
- [ ] Body type is Dynamic (for moving objects)
- [ ] Gravity Scale > 0 (for platformers)

---

### **"Lua error: attempt to index nil value"**

**Cause:** Component doesn't exist.

**Solution:**
```lua
-- BAD: Crashes if component missing
local rb = self.entity:GetComponent_Rigidbody2D()
rb:SetVelocity(...)  -- Crashes if rb is nil

-- GOOD: Check before using
local rb = self.entity:GetComponent_Rigidbody2D()
if rb then
    rb:SetVelocity(...)
end
```

---

### **Editor crashes on startup**

**Possible causes:**
1. OpenGL driver issue
2. Corrupted scene file
3. Missing assets

**Solutions:**
- Update graphics drivers
- Delete `imgui.ini` file
- Start with fresh scene
- Check Console for error messages

---

### **Audio not playing**

**Checklist:**
- [ ] Audio file exists at specified path
- [ ] AudioListener component in scene (add to camera)
- [ ] Volume > 0
- [ ] Not already playing (can't play while playing)

---

### **Entities not colliding**

**Checklist:**
- [ ] Both have Rigidbody2D
- [ ] Both have Colliders
- [ ] Colliders overlap (check sizes)
- [ ] At least one is Dynamic (Static-Static don't collide)

---

## 👨‍💻 **Development & Contributing**

### **Can I contribute to the engine?**

**Yes, contributions are welcome!**

See **[CONTRIBUTING.md](CONTRIBUTING.md)** for:
- Code of conduct
- Contribution workflow
- Coding standards
- How to submit pull requests

---

### **Where is the source code?**

**GitHub:** (Add your repository URL)

```
GameEngine/
├── Engine/          # Core engine source
├── Editor/          # Editor application
├── Sandbox/         # Demo application
└── Vendor/          # Third-party libraries
```

---

### **How do I add a custom component?**

**In C++:**

1. Define in `Engine/include/Engine/Scene/Components.h`:
```cpp
struct MyComponent {
    float MyValue = 0.0f;
};
```

2. Add to scene serializer (optional)
3. Add to properties panel (optional)

See **[USER_GUIDE.md](USER_GUIDE.md)** - Advanced Topics.

---

### **Can I use this for learning?**

**Absolutely!** The engine is perfect for:
- ✅ Learning game engine architecture
- ✅ Understanding ECS (Entity Component System)
- ✅ Learning OpenGL rendering
- ✅ Studying C++17 patterns
- ✅ Game development education

---

### **What's the development roadmap?**

See **[ROADMAP.md](ROADMAP.md)** for full details.

**Upcoming features:**
- 3D rendering support
- Advanced 2D lighting
- Tilemap editor
- Visual scripting
- Networking
- Mobile platforms

---

## 📚 **Documentation & Resources**

### **Where should I start?**

1. **[README.md](README.md)** - Project overview
2. **[QUICKSTART.md](QUICKSTART.md)** - Get engine running
3. **[USER_GUIDE.md](USER_GUIDE.md)** - Complete manual
4. **[QUICK_REFERENCE.md](QUICK_REFERENCE.md)** - Cheat sheet

See **[DOCUMENTATION_INDEX.md](DOCUMENTATION_INDEX.md)** for all docs.

---

### **Is there a tutorial?**

**Yes!**

**Platformer tutorial:** [USER_GUIDE.md](USER_GUIDE.md) - Creating Your First Game  
**Top-down shooter tutorial:** [SHOOTER_BUILD_GUIDE.md](SHOOTER_BUILD_GUIDE.md)

---

### **Where can I get help?**

1. 📖 **Documentation** - Most questions answered here
2. 💬 **GitHub Discussions** - Community Q&A
3. 🐛 **GitHub Issues** - Bug reports
4. 📧 **Email** - See README for contact

---

### **How do I report a bug?**

**GitHub Issues:** (Add your issues URL)

**Include:**
- Engine version
- Operating system
- Steps to reproduce
- Error messages
- Expected vs actual behavior

---

## 🎓 **Learning Resources**

### **I'm new to game development. Where do I start?**

**Recommended path:**
1. **Basic programming** - Learn Lua basics (30 minutes)
2. **Engine concepts** - Read USER_GUIDE.md
3. **Follow tutorial** - Build the platformer
4. **Experiment** - Modify the examples
5. **Build your game** - Start small!

**External resources:**
- [Learn Lua in 15 minutes](http://tylerneylon.com/a/learn-lua/)
- [Game Programming Patterns](https://gameprogrammingpatterns.com/)
- [Learn OpenGL](https://learnopengl.com/) (for engine development)

---

### **What concepts should I understand?**

**Essential:**
- Entities (game objects)
- Components (entity properties)
- Scripts (behavior)
- Delta time (frame-independent movement)

**Important:**
- Physics (forces, collisions)
- Coordinate systems (X, Y, Z)
- Vectors (Vec2, Vec3)
- Game loop (Update, Render)

**Advanced:**
- Entity Component System (ECS)
- Batch rendering
- Spatial audio
- Performance profiling

---

## 💡 **Tips & Best Practices**

### **General Tips**

1. **Save often** - Ctrl+S frequently
2. **Test frequently** - Play button is your friend
3. **Use Console** - Monitor for errors
4. **Check Profiler** - Watch performance
5. **Start simple** - Build complexity gradually

---

### **Scripting Tips**

1. **Cache components** - Get in OnCreate(), use in OnUpdate()
2. **Use local variables** - Faster than globals
3. **Avoid heavy calculations** - Keep OnUpdate() light
4. **Use timers** - Don't check every frame
5. **Print debug info** - Use print() liberally

---

### **Performance Tips**

1. **Use same textures** - Enables batch rendering
2. **Limit particles** - Quality over quantity
3. **Static bodies** - For non-moving objects
4. **Profile first** - Don't optimize blindly
5. **Build Release** - For final testing

---

## 🎉 **Success Stories**

### **What games have been made with this engine?**

The engine is new (v1.0.0 released January 2026), so we're excited to see what YOU create!

**Share your game:**
- Post in GitHub Discussions
- Tag us on social media
- Submit to showcase

---

## ❓ **Still Have Questions?**

**Check these resources:**
1. **[USER_GUIDE.md](USER_GUIDE.md)** - Comprehensive manual
2. **[API_REFERENCE.md](API_REFERENCE.md)** - Complete API docs
3. **[DOCUMENTATION_INDEX.md](DOCUMENTATION_INDEX.md)** - All documentation
4. **GitHub Discussions** - Ask the community
5. **GitHub Issues** - Report bugs

---

**Can't find your answer? Ask in GitHub Discussions!**

---

**FAQ v1.0.0** | **Last Updated:** January 12, 2026  
**Questions? Improvements? Submit a pull request!**

