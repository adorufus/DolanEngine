# 2D Game Engine with OpenGL

A modern C++ game engine built with OpenGL for 2D game development.

## Features (In Progress)

- **Core Systems**
  - ✅ Window management (GLFW)
  - ✅ Event system
  - ✅ Input handling
  - ✅ Logger (spdlog)
  - ✅ Time/Delta time
  - 🚧 Rendering system
  - 🚧 Resource management
  - ⬜ Scene & Entity system
  - ⬜ 2D Physics
  - ⬜ Audio system

## Dependencies

- **GLFW** - Window and input
- **GLAD** - OpenGL loader
- **GLM** - Mathematics library
- **stb_image** - Image loading
- **spdlog** - Logging

## Building

### Prerequisites

- CMake 3.15+
- C++17 compatible compiler
- OpenGL 3.3+

### Clone with Dependencies

```bash
git clone --recursive https://github.com/yourusername/GameEngine.git
cd GameEngine
```

### Build Steps

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Run Sandbox

```bash
./bin/Sandbox  # Linux/macOS
.\bin\Sandbox.exe  # Windows
```

## Project Structure

```
GameEngine/
├── Engine/           # Core engine code
│   ├── include/      # Public headers
│   └── src/          # Implementation
├── Vendor/           # Third-party libraries
├── Sandbox/          # Test/demo application
└── CMakeLists.txt
```

## Usage

```cpp
#include <Engine.h>

class MyGame : public Engine::Application {
public:
    MyGame() : Application("My Game") {
        // Initialize your game
    }
};

Engine::Application* Engine::CreateApplication() {
    return new MyGame();
}
```

## Roadmap

See the development plan for detailed milestone information.

### Phase 1: Foundation ✅
- Core systems
- Window & Events
- Input handling

### Phase 2: Rendering 🚧
- OpenGL abstraction
- 2D renderer
- Batch rendering

### Phase 3: Resources ⬜
- Asset management
- Texture loading
- Resource caching

### Phase 4-8: Advanced Features ⬜
- Entity/Component system
- Physics
- Audio
- Scene management

## License

MIT License - See LICENSE file for details

## Contributing

Contributions are welcome! Please feel free to submit pull requests.

