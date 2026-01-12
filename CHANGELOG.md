# Changelog

All notable changes to the GameEngine project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2026-01-12

### Added
- 🎨 **2D Renderer** with batch rendering
- 🎭 **Entity-Component System** (EnTT-based)
- ⚙️ **Physics Engine** (Box2D integration)
- 🎬 **Animation System** with keyframe support
- ✨ **Particle System** with pooling and emission modes
- 🔊 **Audio Engine** (OpenAL) with 3D spatial audio
- 📜 **Lua Scripting** with hot-reloading
- 🎥 **Camera System** with follow modes and screen shake
- 🖼️ **Editor** with 8 panels:
  - Viewport with Play/Pause/Stop controls
  - Scene Hierarchy with entity tree
  - Properties Inspector for all components
  - Asset Browser with thumbnails
  - Performance Profiler with FPS graphs
  - Console with color-coded logging
  - Renderer Statistics
  - Menu Bar with File operations
- 💾 **Scene Serialization** (YAML format)
- 📊 **Performance Profiler** with real-time monitoring

### Core Features
- Batch rendering (minimize draw calls)
- Texture loading (PNG, JPG)
- Shader system
- Input polling (keyboard, mouse)
- Event system
- Time step management
- Logger with color coding
- Smart pointer utilities (`Ref<T>`, `Scope<T>`)

### Components
- TransformComponent
- SpriteRendererComponent
- CameraComponent
- Rigidbody2DComponent
- BoxCollider2DComponent
- CircleCollider2DComponent
- ScriptComponent
- AnimationComponent
- ParticleEmitterComponent
- AudioSourceComponent
- AudioListenerComponent
- TagComponent

### Dependencies
- GLFW 3.x - Windowing
- GLAD - OpenGL loader
- GLM - Mathematics
- EnTT - ECS framework
- Box2D 2.4.1 - Physics
- yaml-cpp - Serialization
- Dear ImGui 1.90.1 - Editor UI
- Lua 5.4 - Scripting
- sol2 - Lua binding
- OpenAL - Audio
- stb_image - Image loading
- spdlog - Logging

### Performance
- 600+ FPS on demo scene
- ~1.5ms frame time average
- Batch rendering reduces draw calls by 90%+
- Entity pooling for particles

### Documentation
- README.md with quick start
- ARCHITECTURE.md with system design
- QUICKSTART.md for beginners
- SETUP.md for installation
- Phase completion docs (PHASE1-9)
- ROADMAP.md with development timeline

### Examples
- Sandbox demo with physics, particles, audio
- PlayerController.lua script example
- Scene creation examples

## [Unreleased]

### Planned Features
- 3D rendering support
- Advanced 2D lighting (normal maps)
- Tilemap editor
- Visual scripting nodes
- Sprite animation editor
- Networking/multiplayer
- Mobile platform support
- More example games

---

## Version History

- **1.0.0** (2026-01-12) - Initial release
  - Full 2D game engine
  - Complete editor
  - Production-ready

---

For detailed phase-by-phase changes, see:
- [Phase 5](PHASE5_COMPLETE.md) - Advanced Rendering
- [Phase 6](PHASE6_COMPLETE.md) - Audio System
- [Phase 7](PHASE7_COMPLETE.md) - Scripting
- [Phase 8](PHASE8_COMPLETE.md) - Editor & Tools
- [Phase 9](PHASE9_COMPLETE.md) - Optimization

[1.0.0]: https://github.com/yourusername/GameEngine/releases/tag/v1.0.0

