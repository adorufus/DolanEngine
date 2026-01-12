# Engine Architecture

## System Overview

```
┌─────────────────────────────────────────────────────────┐
│                    CLIENT APPLICATION                    │
│                    (Sandbox/MyGame)                      │
└──────────────────────┬──────────────────────────────────┘
                       │ inherits from
                       ↓
┌─────────────────────────────────────────────────────────┐
│                   APPLICATION CORE                       │
│  ┌────────────────────────────────────────────────┐    │
│  │         Application (Main Loop)                 │    │
│  │  - Run()                                        │    │
│  │  - OnEvent()                                    │    │
│  │  - Delta Time Management                       │    │
│  └────────────────────────────────────────────────┘    │
└──────┬────────────┬──────────────┬──────────────────────┘
       │            │              │
       ↓            ↓              ↓
┌──────────┐  ┌──────────┐  ┌──────────┐
│  Window  │  │  Events  │  │  Input   │
└──────────┘  └──────────┘  └──────────┘
       │            │              │
       ↓            ↓              ↓
┌─────────────────────────────────────────────────────────┐
│              PLATFORM ABSTRACTION LAYER                  │
│  ┌──────────────┐  ┌──────────────┐  ┌─────────────┐  │
│  │ GLFWWindow   │  │EventDispatcher│  │ GLFWInput   │  │
│  │ - OpenGL ctx │  │ - Type safe   │  │ - Keyboard  │  │
│  │ - Callbacks  │  │ - Filtering   │  │ - Mouse     │  │
│  └──────────────┘  └──────────────┘  └─────────────┘  │
└──────────┬──────────────────────────────────┬───────────┘
           │                                  │
           ↓                                  ↓
┌─────────────────────┐            ┌──────────────────────┐
│  GLFW + OpenGL      │            │   Third Party Libs   │
│  - Window creation  │            │   - GLM (Math)       │
│  - Input handling   │            │   - spdlog (Log)     │
│  - OpenGL context   │            │   - stb_image        │
└─────────────────────┘            └──────────────────────┘
```

## Data Flow

### Event Flow
```
User Input (Keyboard/Mouse)
    ↓
GLFW Callbacks
    ↓
Event Objects Created (KeyEvent, MouseEvent, etc.)
    ↓
Window::EventCallback
    ↓
Application::OnEvent(Event&)
    ↓
EventDispatcher
    ↓
Specific Handler (OnKeyPressed, OnMouseMoved, etc.)
```

### Main Loop Flow
```
Application::Run()
    ↓
┌─────────────────────────────┐
│  while (m_Running)          │
│    ├─ Calculate DeltaTime   │
│    ├─ Clear Screen          │
│    ├─ Update Logic          │  ← User overrides
│    ├─ Render                │  ← User overrides
│    └─ Window::OnUpdate()    │
│       ├─ Poll Events        │
│       └─ Swap Buffers       │
└─────────────────────────────┘
```

## Class Hierarchy

### Core Classes
```
Application (abstract)
    ├─ Run() [final]
    ├─ OnEvent() [virtual]
    ├─ OnUpdate() [virtual - future]
    └─ OnRender() [virtual - future]
    
Window (interface)
    └─ GLFWWindow (implementation)
        ├─ Init()
        ├─ Shutdown()
        └─ OnUpdate()

Event (abstract)
    ├─ ApplicationEvent
    │   ├─ WindowCloseEvent
    │   └─ WindowResizeEvent
    ├─ KeyEvent (abstract)
    │   ├─ KeyPressedEvent
    │   ├─ KeyReleasedEvent
    │   └─ KeyTypedEvent
    └─ MouseEvent (abstract)
        ├─ MouseMovedEvent
        ├─ MouseScrolledEvent
        ├─ MouseButtonPressedEvent
        └─ MouseButtonReleasedEvent
```

## Module Dependencies

```
┌─────────────────────────────────────────────────┐
│                 Client App                      │
│              (links to Engine)                  │
└──────────────────┬──────────────────────────────┘
                   │ depends on
                   ↓
┌─────────────────────────────────────────────────┐
│            GameEngine (Static Lib)              │
│  ┌──────────────────────────────────────────┐  │
│  │ Core/                                     │  │
│  │  ├─ Application.cpp                      │  │
│  │  └─ Logger.cpp                           │  │
│  │                                           │  │
│  │ Platform/OpenGL/                         │  │
│  │  ├─ GLFWWindow.cpp                       │  │
│  │  └─ GLFWInput.cpp                        │  │
│  │                                           │  │
│  │ Utils/                                    │  │
│  │  └─ stb_image_impl.cpp                   │  │
│  └──────────────────────────────────────────┘  │
└──────────┬──────────────────────────────────────┘
           │ links to
           ↓
┌─────────────────────────────────────────────────┐
│           Third Party Libraries                 │
│  ┌────────┐ ┌─────┐ ┌────────┐ ┌──────────┐   │
│  │  GLFW  │ │GLAD │ │  GLM   │ │  spdlog  │   │
│  │ (3.x)  │ │(3.3)│ │ (Math) │ │  (Log)   │   │
│  └────────┘ └─────┘ └────────┘ └──────────┘   │
└─────────────────────────────────────────────────┘
```

## Memory Management

### Smart Pointer Usage
```cpp
// Scope<T> - Unique ownership (std::unique_ptr)
Scope<Window> m_Window = Window::Create();

// Ref<T> - Shared ownership (std::shared_ptr)
Ref<Texture> m_Texture = Texture::Create("sprite.png");

// Raw pointers only for:
// - Non-owning references
// - Parameters
// - Return values (where appropriate)
```

### Object Lifecycle
```
Application (Created by client)
    └─ Window (Created by Application::Init)
        └─ OpenGL Context (Created by Window::Init)
            └─ GLAD (Initialized by Window::Init)

Destruction order is automatic (RAII):
    GLAD cleanup → OpenGL Context → Window → Application
```

## Thread Model

**Current:** Single-threaded
```
Main Thread:
    ├─ Window/Input polling
    ├─ Event dispatching
    ├─ Game logic update
    └─ Rendering
```

**Future (Phase 9+):**
```
Main Thread:        Update + Render
Asset Thread:       Load textures/sounds
Physics Thread:     Collision detection
Audio Thread:       Sound mixing
```

## File Organization

### Header Organization
```
Engine/include/Engine/
    ├─ Core/           (Core engine functionality)
    │   ├─ Base.h         Platform detection, smart ptrs
    │   ├─ Application.h  Main application class
    │   ├─ Window.h       Window interface
    │   ├─ Input.h        Input queries
    │   ├─ Logger.h       Logging macros
    │   ├─ TimeStep.h     Delta time
    │   └─ EntryPoint.h   main() function
    │
    └─ Events/         (Event system)
        ├─ Event.h           Base event + dispatcher
        ├─ ApplicationEvent.h Window events
        ├─ KeyEvent.h        Keyboard events
        └─ MouseEvent.h      Mouse events
```

### Implementation Organization
```
Engine/src/
    ├─ Core/           (Core implementations)
    │   ├─ Application.cpp
    │   └─ Logger.cpp
    │
    ├─ Platform/       (Platform-specific code)
    │   └─ OpenGL/
    │       ├─ GLFWWindow.cpp    (GLFW implementation)
    │       └─ GLFWInput.cpp     (GLFW input)
    │
    └─ Utils/          (Utilities)
        └─ stb_image_impl.cpp
```

## Design Patterns Used

### 1. Singleton Pattern
```cpp
class Application {
    static Application* s_Instance;
public:
    static Application& Get() { return *s_Instance; }
};
```

### 2. Strategy Pattern
```cpp
class Window {  // Interface
    virtual void OnUpdate() = 0;
};

class GLFWWindow : public Window {  // Strategy
    void OnUpdate() override { /* GLFW impl */ }
};
```

### 3. Observer Pattern (Events)
```cpp
// Event = Subject
// EventCallback = Observer
window.SetEventCallback([](Event& e) {
    // React to event
});
```

### 4. Template Method Pattern
```cpp
class Application {
    void Run() {  // Template method
        while (running) {
            OnUpdate();   // Hook
            OnRender();   // Hook
        }
    }
};
```

## Platform Abstraction

### Current Abstraction Points
```
┌──────────────────┐
│   Application    │  ← Platform independent
├──────────────────┤
│   Window API     │  ← Abstract interface
├──────────────────┤
│  GLFWWindow      │  ← Platform specific
├──────────────────┤
│   GLFW + OS      │  ← Platform layer
└──────────────────┘
```

### Adding New Platform
```cpp
// 1. Create new implementation
class SDLWindow : public Window {
    void Init() override;
    void OnUpdate() override;
    // ... implement interface
};

// 2. Update factory
Scope<Window> Window::Create() {
    #ifdef USE_SDL
        return CreateScope<SDLWindow>();
    #else
        return CreateScope<GLFWWindow>();
    #endif
}
```

## Configuration System

### Compile-Time Configuration
```cpp
// Engine/Core/Base.h
#ifdef GE_DEBUG
    #define GE_ENABLE_ASSERTS
    #define GE_DEBUGBREAK() raise(SIGTRAP)
#endif

#ifdef GE_PLATFORM_MACOS
    // macOS specific code
#endif
```

### Runtime Configuration
```cpp
WindowProps props;
props.Width = 1920;
props.Height = 1080;
props.VSync = true;
props.Title = "My Game";

m_Window = Window::Create(props);
```

## Error Handling

### Assertions (Debug builds)
```cpp
GE_ASSERT(condition, "Error message");
// Logs error and triggers debugger
```

### Logging
```cpp
GE_CORE_ERROR("Critical engine error: {}", details);
GE_ERROR("Application error: {}", details);
GE_WARN("Warning: {}", details);
GE_INFO("Info: {}", details);
```

## Build System Architecture

```
CMakeLists.txt (Root)
    ├─ Vendor/CMakeLists.txt
    │   ├─ GLFW (subdirectory)
    │   ├─ glad (library)
    │   ├─ GLM (interface)
    │   ├─ spdlog (interface)
    │   └─ stb_image (interface)
    │
    ├─ Engine/CMakeLists.txt
    │   └─ GameEngine (static library)
    │       ├─ Sources: src/**/*.cpp
    │       ├─ Headers: include/**/*.h
    │       └─ Links: glfw, glad, glm, spdlog
    │
    └─ Sandbox/CMakeLists.txt
        └─ Sandbox (executable)
            └─ Links: GameEngine
```

## Extension Points (Future)

### Layer System (Phase 2+)
```cpp
class Application {
    LayerStack m_LayerStack;
    
    void PushLayer(Layer* layer);
    void PopLayer(Layer* layer);
};
```

### Renderer (Phase 2+)
```cpp
class Renderer {
    static void BeginScene(Camera& camera);
    static void Submit(VertexArray& va, Shader& shader);
    static void EndScene();
};
```

### Entity Component System (Phase 4+)
```cpp
class Entity {
    template<typename T>
    T& AddComponent();
    
    template<typename T>
    T& GetComponent();
};
```

## Performance Considerations

### Current
- **Event System**: Zero-cost when events not used
- **Logging**: Can be compiled out in release builds
- **Smart Pointers**: Minimal overhead over raw pointers

### Future Optimizations
- **Batch Rendering**: Minimize draw calls
- **Object Pooling**: Reduce allocations
- **Spatial Partitioning**: For collision detection
- **Asset Caching**: Prevent redundant loads

## Summary

The engine follows a **layered architecture** with clear separation between:
1. **Client Application** - User's game code
2. **Engine Core** - Platform-independent logic
3. **Platform Layer** - OS/library specific code
4. **Third-Party** - External dependencies

This design allows for:
- ✅ Easy testing and debugging
- ✅ Platform portability
- ✅ Clean extension points
- ✅ Maintainable codebase

