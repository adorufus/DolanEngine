# Quick Start Guide

Get the engine running in 5 minutes!

## 1. Install CMake

**macOS:**
```bash
brew install cmake
```

**Linux:**
```bash
sudo apt-get install cmake build-essential libgl1-mesa-dev
```

**Windows:**
Download from https://cmake.org/download/

## 2. Clone and Build

```bash
cd GameEngine

# All dependencies are already set up!
# Just build:
mkdir build && cd build
cmake ..
cmake --build .
```

## 3. Run

```bash
# macOS/Linux
./bin/Sandbox

# Windows
.\bin\Debug\Sandbox.exe
```

You should see a window with a dark gray background. Press ESC to exit.

## What You Just Built

✅ **Window System** - GLFW-based window with events
✅ **OpenGL Context** - OpenGL 3.3 Core initialized
✅ **Input System** - Keyboard and mouse support
✅ **Event System** - Window and input events
✅ **Logger** - Color-coded logging with spdlog
✅ **Time System** - Delta time tracking

## Next: Test Input

Modify `Sandbox/src/main.cpp`:

```cpp
#include <Engine.h>

class SandboxApp : public Engine::Application {
public:
    SandboxApp() : Application("Sandbox") {
        GE_INFO("Sandbox created!");
    }
    
    void OnUpdate(float deltaTime) {
        if (Engine::Input::IsKeyPressed(Engine::KeyCode::Escape)) {
            GE_INFO("Escape pressed!");
        }
    }
};

Engine::Application* Engine::CreateApplication() {
    return new SandboxApp();
}
```

Rebuild and run to test input!

## Project Structure

```
GameEngine/
├── Engine/          # Core engine (static library)
├── Sandbox/         # Test application
├── Vendor/          # Third-party libs (GLFW, GLM, etc.)
└── build/           # Build output
    └── bin/
        └── Sandbox  # Your executable
```

## What's Next?

Phase 2 is next: **Rendering System**
- OpenGL abstraction (Shaders, Textures, Buffers)
- 2D Renderer with batch rendering
- Camera system
- Sprite rendering

See `README.md` for the full roadmap!

