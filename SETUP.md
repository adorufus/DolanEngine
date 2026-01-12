# Setup Instructions

## Installing Dependencies

This project uses several third-party libraries. Here's how to set them up:

### Option 1: Git Submodules (Recommended)

```bash
# Add GLFW
git submodule add https://github.com/glfw/glfw.git Vendor/glfw

# Add GLM
git submodule add https://github.com/g-truc/glm.git Vendor/glm

# Add spdlog
git submodule add https://github.com/gabime/spdlog.git Vendor/spdlog

# Initialize all submodules
git submodule update --init --recursive
```

### Option 2: Manual Download

1. **GLFW** - Download from https://github.com/glfw/glfw
   - Extract to `Vendor/glfw/`

2. **GLM** - Download from https://github.com/g-truc/glm
   - Extract to `Vendor/glm/`

3. **spdlog** - Download from https://github.com/gabime/spdlog
   - Extract to `Vendor/spdlog/`

### GLAD Setup

GLAD needs to be generated:

1. Go to https://glad.dav1d.de/
2. Settings:
   - Language: C/C++
   - API gl: Version 3.3+
   - Profile: Core
3. Generate and download
4. Extract to `Vendor/glad/`
   - `Vendor/glad/include/` (glad.h, KHR/)
   - `Vendor/glad/src/glad.c`

### STB Image

1. Download `stb_image.h` from https://github.com/nothings/stb
2. Create `Vendor/stb/stb_image.h`
3. Create implementation file `Engine/src/Utils/stb_image_impl.cpp`:

```cpp
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
```

## Platform-Specific Notes

### macOS

```bash
# Install Xcode Command Line Tools
xcode-select --install
```

### Linux (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install build-essential cmake
sudo apt-get install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
sudo apt-get install mesa-common-dev libgl1-mesa-dev
```

### Windows

- Install Visual Studio 2019+ with C++ development tools
- CMake 3.15+
- Git for Windows (if using submodules)

## Building

```bash
# Create build directory
mkdir build && cd build

# Generate build files
cmake ..

# Build (choose one)
cmake --build .                    # All platforms
cmake --build . --config Release   # For multi-config generators (VS)
make                               # Linux/macOS with Makefile

# Run
./bin/Sandbox          # Linux/macOS
.\bin\Debug\Sandbox.exe   # Windows (Debug)
```

## Troubleshooting

### "GLFW not found"
- Ensure GLFW is in `Vendor/glfw/` directory
- Check CMakeLists.txt path

### "GLAD errors"
- Make sure GLAD is properly generated for OpenGL 3.3 Core
- Check include paths

### OpenGL version issues on macOS
- macOS supports up to OpenGL 4.1
- Engine is set to use 3.3 Core for compatibility

### Linking errors
- Make sure all libraries are built for the same architecture (x64)
- Check CMake output for missing dependencies

